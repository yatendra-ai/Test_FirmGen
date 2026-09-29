#ifndef FG_ML_STAI_H
#define FG_ML_STAI_H

/* Optional binding for the documented STAI 1.x client API and default generated
 * C-name "network". This is not an LL_ATON or legacy ai_network_* adapter.
 * https://stedgeai-dc.st.com/assets/embedded-docs/embedded_client_stai_api.html
 */
#include "fg_ml_runtime.h"
#include "network.h"

#if !defined(STAI_API_VERSION_MAJOR) || STAI_API_VERSION_MAJOR != 1
#error "fg_ml_stai requires the documented STAI client API major version 1"
#endif
#if !defined(STAI_NETWORK_CONTEXT_SIZE) || !defined(STAI_NETWORK_CONTEXT_ALIGNMENT) || \
    !defined(STAI_NETWORK_IN_NUM) || !defined(STAI_NETWORK_OUT_NUM) || \
    !defined(STAI_NETWORK_ACTIVATIONS_NUM) || !defined(STAI_NETWORK_STATES_NUM) || \
    !defined(STAI_NETWORK_WEIGHTS_NUM)
#error "Unsupported generated network.h: requires STAI_NETWORK metadata (not legacy/LL_ATON-only)"
#endif
#if STAI_NETWORK_IN_NUM < 1 || STAI_NETWORK_OUT_NUM < 1
#error "fg_ml_stai requires at least one model input and output"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    stai_network *network_context;
    size_t context_size_bytes;
    const fg_ml_tensor_t *activations;
    size_t activation_count;
    const fg_ml_tensor_t *states;
    size_t state_count;
    /* NULL + zero count retains generated embedded weights. Otherwise every
     * generated weight buffer must be supplied and already loaded by the board. */
    const fg_ml_tensor_t *weights;
    size_t weight_count;
    /* 1: this binding is sole owner of stai_runtime_init/deinit. 0: board owns
     * global runtime lifetime and must initialize it before fg_ml_init. */
    int own_runtime;
} fg_ml_stai_config_t;

typedef struct {
    fg_ml_stai_config_t config;
    stai_ptr activation_ptrs[STAI_NETWORK_ACTIVATIONS_NUM > 0 ? STAI_NETWORK_ACTIVATIONS_NUM : 1];
    stai_ptr state_ptrs[STAI_NETWORK_STATES_NUM > 0 ? STAI_NETWORK_STATES_NUM : 1];
    stai_ptr weight_ptrs[STAI_NETWORK_WEIGHTS_NUM > 0 ? STAI_NETWORK_WEIGHTS_NUM : 1];
    stai_ptr input_ptrs[STAI_NETWORK_IN_NUM];
    stai_ptr output_ptrs[STAI_NETWORK_OUT_NUM];
    stai_return_code last_vendor_error;
    int runtime_initialized;
    int network_initialized;
    int ready;
} fg_ml_stai_binding_t;

/* Zero-initialize binding and generic runtime before configuring. Buffer
 * descriptors and pointed-to memory must outlive initialization/inference.
 * Alignment and sizes come exclusively from generated STAI_NETWORK_* macros.
 * Heap metadata uses SIZES_BYTES when available, otherwise SIZES only after
 * verifying the generated aggregate SIZE and SIZE_BYTES units agree.
 * No allocation, quantization, clock/cache setup, address selection or flashing
 * is performed here. Calls are synchronous and must be serialized by the board.
 * On vendor failure callbacks return FG_ML_STAI_VENDOR_ERROR; inspect the exact
 * unmodified stai_return_code with fg_ml_stai_last_error(). */
enum { FG_ML_STAI_VENDOR_ERROR = -1000 };
int fg_ml_stai_configure(fg_ml_stai_binding_t *binding,
                         const fg_ml_stai_config_t *config,
                         fg_ml_backend_t *backend);
stai_return_code fg_ml_stai_last_error(const fg_ml_stai_binding_t *binding);

#ifdef __cplusplus
}
#endif
#endif
