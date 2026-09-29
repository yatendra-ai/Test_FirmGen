/*
 * @file c5_ml_quick_test.c
 * @brief Static-storage ST Edge AI 4.0 runtime adapter for the generated CPU model.
 *
 * The selected generated C API uses STAI v1. The generated map specifies 16 bytes
 * of activations and 24 bytes of weights; network_data.c embeds the latter in ROM.
 * All storage is fixed and the generated model implementation is sourced once
 * from the matching model bundle; analysis workspaces are kept outside firmware/.
 */
#include "c5_ml_quick_test.h"

#include <stdint.h>
#include <string.h>

#include "fg_ml_runtime.h"
#include "fg_ml_stai.h"
#include "network.h"

#define C5_INPUT_ELEMENT_COUNT 2U
#define C5_OUTPUT_ELEMENT_COUNT 2U

_Static_assert(STAI_NETWORK_IN_NUM == 1, "Exactly one ST AI input is required");
_Static_assert(STAI_NETWORK_OUT_NUM == 1, "Exactly one ST AI output is required");
_Static_assert(STAI_NETWORK_IN_1_SIZE_BYTES == C5_INPUT_ELEMENT_COUNT * sizeof(float), "Unexpected ST AI input size");
_Static_assert(STAI_NETWORK_OUT_1_SIZE_BYTES == C5_OUTPUT_ELEMENT_COUNT * sizeof(float), "Unexpected ST AI output size");
_Static_assert(STAI_NETWORK_ACTIVATIONS_NUM == 1, "Expected one activation buffer");
_Static_assert(STAI_NETWORK_STATES_NUM == 0, "Expected no persistent state buffers");

static STAI_ALIGNED(STAI_NETWORK_CONTEXT_ALIGNMENT)
uint8_t s_network_context[STAI_NETWORK_CONTEXT_SIZE];

static STAI_ALIGNED(STAI_NETWORK_ACTIVATION_1_ALIGNMENT)
uint8_t s_network_activations[STAI_NETWORK_ACTIVATION_1_SIZE_BYTES];

static STAI_ALIGNED(STAI_NETWORK_IN_1_ALIGNMENT)
float s_network_input[C5_INPUT_ELEMENT_COUNT];

static STAI_ALIGNED(STAI_NETWORK_OUT_1_ALIGNMENT)
float s_network_output[C5_OUTPUT_ELEMENT_COUNT];

static fg_ml_runtime_t s_runtime;
static fg_ml_stai_binding_t s_binding;
static fg_ml_backend_t s_backend;
static bool s_runtime_initialized;
static bool s_cleanup_succeeded;

static const fg_ml_tensor_t s_activation_tensor = {
    .data = s_network_activations,
    .size_bytes = sizeof(s_network_activations)
};

/**
 * @brief Configures and initializes the ST AI runtime with embedded model weights.
 *
 * @return true when both runtime and generated model initialize successfully.
 */
bool c5_ml_runtime_init(void)
{
    const fg_ml_stai_config_t stai_config = {
        .network_context = (stai_network *)s_network_context,
        .context_size_bytes = sizeof(s_network_context),
        .activations = &s_activation_tensor,
        .activation_count = STAI_NETWORK_ACTIVATIONS_NUM,
        .states = NULL,
        .state_count = 0U,
        .weights = NULL,
        .weight_count = 0U,
        .own_runtime = 1
    };

    if (s_runtime_initialized)
    {
        return false;
    }

    (void)memset(&s_runtime, 0, sizeof(s_runtime));
    (void)memset(&s_binding, 0, sizeof(s_binding));
    (void)memset(&s_backend, 0, sizeof(s_backend));
    (void)memset(s_network_context, 0, sizeof(s_network_context));
    (void)memset(s_network_activations, 0, sizeof(s_network_activations));

    if (fg_ml_stai_configure(&s_binding, &stai_config, &s_backend) != FG_ML_OK)
    {
        return false;
    }

    if (fg_ml_init(&s_runtime, &s_backend) != FG_ML_OK)
    {
        if ((s_runtime.initialized != 0) && (s_runtime.backend.deinit != NULL))
        {
            s_runtime.backend.deinit(s_runtime.backend.context);
            s_runtime.initialized = 0;
        }
        s_cleanup_succeeded = (s_binding.last_vendor_error == STAI_SUCCESS) &&
                              (s_binding.network_initialized == 0) &&
                              (s_binding.runtime_initialized == 0);
        return false;
    }

    s_runtime_initialized = true;
    s_cleanup_succeeded = false;
    return true;
}

bool c5_ml_infer(const float input[C5_INPUT_ELEMENT_COUNT],
                 float output[C5_OUTPUT_ELEMENT_COUNT])
{
    const fg_ml_tensor_t input_tensor = {
        .data = s_network_input,
        .size_bytes = sizeof(s_network_input)
    };
    fg_ml_tensor_t output_tensor = {
        .data = s_network_output,
        .size_bytes = sizeof(s_network_output)
    };

    if (!s_runtime_initialized || (input == NULL) || (output == NULL))
    {
        return false;
    }

    s_network_input[0] = input[0];
    s_network_input[1] = input[1];

    if (fg_ml_run(&s_runtime, &input_tensor, 1U, &output_tensor, 1U) != FG_ML_OK)
    {
        return false;
    }

    output[0] = s_network_output[0];
    output[1] = s_network_output[1];
    return true;
}

bool c5_ml_runtime_deinit(void)
{
    if (!s_runtime_initialized)
    {
        return s_cleanup_succeeded;
    }

    fg_ml_deinit(&s_runtime);
    s_runtime_initialized = false;
    s_cleanup_succeeded = (s_binding.last_vendor_error == STAI_SUCCESS) &&
                          (s_binding.network_initialized == 0) &&
                          (s_binding.runtime_initialized == 0);
    return s_cleanup_succeeded;
}
