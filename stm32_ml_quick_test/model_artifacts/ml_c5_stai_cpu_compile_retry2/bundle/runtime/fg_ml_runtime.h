#ifndef FG_ML_RUNTIME_H
#define FG_ML_RUNTIME_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct { void *data; size_t size_bytes; } fg_ml_tensor_t;
typedef struct {
    void *context;
    int (*init)(void *context);
    int (*run)(void *context, const fg_ml_tensor_t *inputs, size_t input_count,
               fg_ml_tensor_t *outputs, size_t output_count);
    void (*deinit)(void *context);
} fg_ml_backend_t;
typedef struct { fg_ml_backend_t backend; int initialized; } fg_ml_runtime_t;
enum { FG_ML_OK = 0, FG_ML_INVALID = -1, FG_ML_NOT_INITIALIZED = -2 };

/* Zero-initialize runtime before the first init. Backend retains ownership of
 * buffers, alignment, accelerator/cache setup and memory pools. Non-reentrant. */
int fg_ml_init(fg_ml_runtime_t *runtime, const fg_ml_backend_t *backend);
int fg_ml_run(fg_ml_runtime_t *runtime, const fg_ml_tensor_t *inputs, size_t input_count,
              fg_ml_tensor_t *outputs, size_t output_count);
void fg_ml_deinit(fg_ml_runtime_t *runtime);
/* Explicit optional processing primitives; no automatic model transformations. */
int fg_ml_prepare_input(const float *source, float *destination, size_t count,
                        float scale, float offset);
int fg_ml_process_output(const float *scores, size_t count, size_t *class_index);
#ifdef __cplusplus
}
#endif
#endif
