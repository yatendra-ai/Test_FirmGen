#include "fg_ml_runtime.h"
#include <math.h>

int fg_ml_init(fg_ml_runtime_t *r, const fg_ml_backend_t *b) {
    int status;
    if (!r || !b || !b->init || !b->run || r->initialized) return FG_ML_INVALID;
    r->backend = *b;
    status = b->init(b->context);
    r->initialized = (status == FG_ML_OK);
    return status;
}
int fg_ml_run(fg_ml_runtime_t *r, const fg_ml_tensor_t *in, size_t ni,
              fg_ml_tensor_t *out, size_t no) {
    size_t i;
    if (!r || !r->initialized) return FG_ML_NOT_INITIALIZED;
    if (!in || !out || !ni || !no) return FG_ML_INVALID;
    for (i = 0; i < ni; ++i) if (!in[i].data || !in[i].size_bytes) return FG_ML_INVALID;
    for (i = 0; i < no; ++i) if (!out[i].data || !out[i].size_bytes) return FG_ML_INVALID;
    return r->backend.run(r->backend.context, in, ni, out, no);
}
void fg_ml_deinit(fg_ml_runtime_t *r) {
    if (r && r->initialized) {
        if (r->backend.deinit) r->backend.deinit(r->backend.context);
        r->initialized = 0;
    }
}
int fg_ml_prepare_input(const float *src, float *dst, size_t n, float scale, float offset) {
    size_t i;
    if (!src || !dst || !n || !isfinite(scale) || !isfinite(offset)) return FG_ML_INVALID;
    for (i = 0; i < n; ++i) {
        if (!isfinite(src[i]) || !isfinite(src[i] * scale + offset)) return FG_ML_INVALID;
    }
    for (i = 0; i < n; ++i) dst[i] = src[i] * scale + offset;
    return FG_ML_OK;
}
int fg_ml_process_output(const float *scores, size_t n, size_t *index) {
    size_t i, best = 0;
    if (!scores || !index || !n) return FG_ML_INVALID;
    for (i = 0; i < n; ++i) {
        if (!isfinite(scores[i])) return FG_ML_INVALID;
        if (scores[i] > scores[best]) best = i;
    }
    *index = best;
    return FG_ML_OK;
}
