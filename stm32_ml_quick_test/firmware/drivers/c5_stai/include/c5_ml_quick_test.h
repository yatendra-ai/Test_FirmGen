/*
 * @file c5_ml_quick_test.h
 * @brief Static-storage lifecycle and inference API for the generated C5 ST AI model.
 */
#ifndef C5_ML_QUICK_TEST_H
#define C5_ML_QUICK_TEST_H

#include <stdbool.h>

/**
 * @brief Initializes the generated ST AI runtime, model, and fixed buffers.
 *
 * @return true when runtime, network context, and activation storage initialize
 * successfully; false when any ST AI lifecycle operation fails.
 * @pre Board clock and Cortex-M33 FPU initialization have completed.
 * @post On success, the runtime remains initialized until c5_ml_runtime_deinit().
 */
bool c5_ml_runtime_init(void);

/**
 * @brief Runs one direct FP32 inference using the generated ST AI network.
 *
 * @param[in] input Two FP32 values for the generated network input tensor.
 * @param[out] output Two FP32 values retrieved directly from the generated output tensor.
 * @return true when the synchronous ST AI inference completes successfully.
 * @pre c5_ml_runtime_init() completed successfully; input and output each reference
 * at least two float elements and are 4-byte aligned.
 * @note No preprocessing or postprocessing is performed.
 */
bool c5_ml_infer(const float input[2], float output[2]);

/**
 * @brief Deinitializes the generated network and ST AI runtime.
 *
 * @return true when all owned ST AI resources deinitialize successfully.
 * @pre c5_ml_runtime_init() succeeded.
 * @post Runtime/network ownership is released and a new initialization may start.
 */
bool c5_ml_runtime_deinit(void);

#endif /* C5_ML_QUICK_TEST_H */
