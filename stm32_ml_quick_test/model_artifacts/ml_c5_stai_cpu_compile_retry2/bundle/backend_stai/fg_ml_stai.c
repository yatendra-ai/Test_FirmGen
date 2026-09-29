#include "fg_ml_stai.h"
#include <stdint.h>

static const size_t input_sizes[] = STAI_NETWORK_IN_SIZES_BYTES;
static const size_t input_alignments[] = STAI_NETWORK_IN_ALIGNMENTS;
static const size_t output_sizes[] = STAI_NETWORK_OUT_SIZES_BYTES;
static const size_t output_alignments[] = STAI_NETWORK_OUT_ALIGNMENTS;
#if STAI_NETWORK_ACTIVATIONS_NUM > 0
#if defined(STAI_NETWORK_ACTIVATIONS_SIZES_BYTES)
static const size_t activation_sizes[] = STAI_NETWORK_ACTIVATIONS_SIZES_BYTES;
#else
/* Core 4.0.1 emits _SIZES for byte heaps, without plural _SIZES_BYTES.
 * Accept that form only when generated aggregate units confirm byte sizes. */
_Static_assert(STAI_NETWORK_ACTIVATIONS_SIZE == STAI_NETWORK_ACTIVATIONS_SIZE_BYTES,
               "Activation metadata must describe byte heaps or provide SIZES_BYTES");
static const size_t activation_sizes[] = STAI_NETWORK_ACTIVATIONS_SIZES;
#endif
static const size_t activation_alignments[] = STAI_NETWORK_ACTIVATIONS_ALIGNMENTS;
#endif
#if STAI_NETWORK_STATES_NUM > 0
#if defined(STAI_NETWORK_STATES_SIZES_BYTES)
static const size_t state_sizes[] = STAI_NETWORK_STATES_SIZES_BYTES;
#else
_Static_assert(STAI_NETWORK_STATES_SIZE == STAI_NETWORK_STATES_SIZE_BYTES,
               "State metadata must describe byte heaps or provide SIZES_BYTES");
static const size_t state_sizes[] = STAI_NETWORK_STATES_SIZES;
#endif
static const size_t state_alignments[] = STAI_NETWORK_STATES_ALIGNMENTS;
#endif
#if STAI_NETWORK_WEIGHTS_NUM > 0
#if defined(STAI_NETWORK_WEIGHTS_SIZES_BYTES)
static const size_t weight_sizes[] = STAI_NETWORK_WEIGHTS_SIZES_BYTES;
#else
_Static_assert(STAI_NETWORK_WEIGHTS_SIZE == STAI_NETWORK_WEIGHTS_SIZE_BYTES,
               "Weight metadata must describe byte heaps or provide SIZES_BYTES");
static const size_t weight_sizes[] = STAI_NETWORK_WEIGHTS_SIZES;
#endif
static const size_t weight_alignments[] = STAI_NETWORK_WEIGHTS_ALIGNMENTS;
#endif

static int aligned(const void *pointer, size_t alignment) {
    return pointer && alignment && ((uintptr_t)pointer % alignment == 0);
}

static int buffers(const fg_ml_tensor_t *items, size_t count, size_t expected,
                   const size_t *sizes, const size_t *alignments,
                   stai_ptr *pointers, int exact_size) {
    size_t i;
    if (count != expected || (count && !items)) return FG_ML_INVALID;
    for (i = 0; i < count; ++i) {
        if (!aligned(items[i].data, alignments[i]) ||
            (exact_size ? items[i].size_bytes != sizes[i] : items[i].size_bytes < sizes[i]))
            return FG_ML_INVALID;
    }
    for (i = 0; i < count; ++i) pointers[i] = (stai_ptr)items[i].data;
    return FG_ML_OK;
}

static stai_return_code release(fg_ml_stai_binding_t *binding) {
    stai_return_code rc;
    binding->ready = 0;
    if (binding->network_initialized) {
        rc = stai_network_deinit(binding->config.network_context);
        if (rc != STAI_SUCCESS) return rc;
        binding->network_initialized = 0;
    }
    if (binding->runtime_initialized) {
        rc = stai_runtime_deinit();
        if (rc != STAI_SUCCESS) return rc;
        binding->runtime_initialized = 0;
    }
    return STAI_SUCCESS;
}

static int vendor_failure(fg_ml_stai_binding_t *binding, stai_return_code rc, int cleanup) {
    if (cleanup) (void)release(binding);
    binding->last_vendor_error = rc;
    return FG_ML_STAI_VENDOR_ERROR;
}

static int initialize(void *context) {
    fg_ml_stai_binding_t *binding = (fg_ml_stai_binding_t *)context;
    const fg_ml_stai_config_t *config;
    stai_return_code rc;
    size_t i;
    if (!binding || binding->ready || binding->network_initialized || binding->runtime_initialized)
        return FG_ML_INVALID;
    config = &binding->config;
    if (!aligned(config->network_context, STAI_NETWORK_CONTEXT_ALIGNMENT) ||
        config->context_size_bytes < STAI_NETWORK_CONTEXT_SIZE ||
        (config->own_runtime != 0 && config->own_runtime != 1)) return FG_ML_INVALID;
#if STAI_NETWORK_ACTIVATIONS_NUM > 0
    if (buffers(config->activations, config->activation_count, STAI_NETWORK_ACTIVATIONS_NUM,
                activation_sizes, activation_alignments, binding->activation_ptrs, 0) != FG_ML_OK)
        return FG_ML_INVALID;
#else
    if (config->activation_count) return FG_ML_INVALID;
#endif
#if STAI_NETWORK_STATES_NUM > 0
    if (buffers(config->states, config->state_count, STAI_NETWORK_STATES_NUM,
                state_sizes, state_alignments, binding->state_ptrs, 0) != FG_ML_OK)
        return FG_ML_INVALID;
#else
    if (config->state_count) return FG_ML_INVALID;
#endif
    if (config->weight_count || config->weights) {
#if STAI_NETWORK_WEIGHTS_NUM > 0
        if (buffers(config->weights, config->weight_count, STAI_NETWORK_WEIGHTS_NUM,
                    weight_sizes, weight_alignments, binding->weight_ptrs, 0) != FG_ML_OK)
            return FG_ML_INVALID;
#else
        return FG_ML_INVALID;
#endif
    }
    binding->last_vendor_error = STAI_SUCCESS;
    if (config->own_runtime) {
        rc = stai_runtime_init();
        if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 0);
        binding->runtime_initialized = 1;
    }
    /* Context is documented as a byte buffer, including in generated examples. */
    for (i = 0; i < STAI_NETWORK_CONTEXT_SIZE; ++i)
        ((unsigned char *)config->network_context)[i] = 0;
    rc = stai_network_init(config->network_context);
    if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 1);
    binding->network_initialized = 1;
#if STAI_NETWORK_ACTIVATIONS_NUM > 0
    rc = stai_network_set_activations(config->network_context, binding->activation_ptrs,
                                      STAI_NETWORK_ACTIVATIONS_NUM);
    if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 1);
#endif
#if STAI_NETWORK_STATES_NUM > 0
    rc = stai_network_set_states(config->network_context, binding->state_ptrs, STAI_NETWORK_STATES_NUM);
    if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 1);
#endif
#if STAI_NETWORK_WEIGHTS_NUM > 0
    if (config->weight_count) {
        rc = stai_network_set_weights(config->network_context, binding->weight_ptrs, STAI_NETWORK_WEIGHTS_NUM);
        if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 1);
    }
#endif
    binding->ready = 1;
    return FG_ML_OK;
}

static int run(void *context, const fg_ml_tensor_t *inputs, size_t input_count,
               fg_ml_tensor_t *outputs, size_t output_count) {
    fg_ml_stai_binding_t *binding = (fg_ml_stai_binding_t *)context;
    stai_return_code rc;
    if (!binding || !binding->ready) return FG_ML_NOT_INITIALIZED;
    if (buffers(inputs, input_count, STAI_NETWORK_IN_NUM, input_sizes, input_alignments,
                binding->input_ptrs, 1) != FG_ML_OK ||
        buffers(outputs, output_count, STAI_NETWORK_OUT_NUM, output_sizes, output_alignments,
                binding->output_ptrs, 1) != FG_ML_OK) return FG_ML_INVALID;
    rc = stai_network_set_inputs(binding->config.network_context, binding->input_ptrs, STAI_NETWORK_IN_NUM);
    if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 0);
    rc = stai_network_set_outputs(binding->config.network_context, binding->output_ptrs, STAI_NETWORK_OUT_NUM);
    if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 0);
    rc = stai_network_run(binding->config.network_context, STAI_MODE_SYNC);
    if (rc != STAI_SUCCESS) return vendor_failure(binding, rc, 0);
    binding->last_vendor_error = STAI_SUCCESS;
    return FG_ML_OK;
}

static void deinitialize(void *context) {
    fg_ml_stai_binding_t *binding = (fg_ml_stai_binding_t *)context;
    if (binding) binding->last_vendor_error = release(binding);
}

int fg_ml_stai_configure(fg_ml_stai_binding_t *binding,
                         const fg_ml_stai_config_t *config, fg_ml_backend_t *backend) {
    if (!binding || !config || !backend || binding->ready ||
        binding->network_initialized || binding->runtime_initialized) return FG_ML_INVALID;
    binding->config = *config;
    binding->last_vendor_error = STAI_SUCCESS;
    backend->context = binding;
    backend->init = initialize;
    backend->run = run;
    backend->deinit = deinitialize;
    return FG_ML_OK;
}

stai_return_code fg_ml_stai_last_error(const fg_ml_stai_binding_t *binding) {
    return binding ? binding->last_vendor_error : STAI_ERROR_NETWORK_INVALID_CONTEXT_HANDLE;
}
