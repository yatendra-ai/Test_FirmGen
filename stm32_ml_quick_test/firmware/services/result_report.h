/*
 * @file result_report.h
 * @brief Deterministic ML startup, vector, failure, and summary reporting API.
 */
#ifndef RESULT_REPORT_H
#define RESULT_REPORT_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Resets the reporter's per-run readiness and failure state.
 *
 * @post No model digest or runtime-ready state is assumed for the new run.
 */
void result_report_reset(void);

/**
 * @brief Prints the SHA-256 identifier of the model intended for this run.
 *
 * @param[in] sha256_hex Exactly 64 hexadecimal characters identifying the model;
 * NULL or malformed values are rejected and reported as a startup failure.
 * @return true only when the digest is valid and the complete line was transmitted.
 * @post A successful call permits a later result_report_runtime_ready() call.
 */
bool result_report_startup(const char *sha256_hex);

/**
 * @brief Prints runtime readiness after model and runtime initialization succeed.
 *
 * @return true when the readiness line was transmitted; false if startup did not
 * succeed or the console transmission failed.
 * @pre result_report_startup() returned true and model/runtime initialization succeeded.
 */
bool result_report_runtime_ready(void);

/**
 * @brief Prints one deterministic comparison line for an executed vector.
 *
 * @param[in] vector_id Reference-vector identifier containing only letters,
 * digits, and underscores.
 * @param[in] output_0_milli First FP32 output multiplied by 1000 and converted to integer.
 * @param[in] output_1_milli Second FP32 output multiplied by 1000 and converted to integer.
 * @param[in] comparison_passed Whether both unformatted FP32 comparisons passed.
 * @return true when the complete result line was transmitted; false on invalid
 * input, unavailable runtime readiness, or console transmission failure.
 * @pre The vector was executed and both original FP32 outputs were compared.
 */
bool result_report_vector(const char *vector_id,
                          int32_t output_0_milli,
                          int32_t output_1_milli,
                          bool comparison_passed);

/**
 * @brief Reports a named runtime or test failure when the console remains usable.
 *
 * @param[in] reason Short identifier containing only letters, digits, and underscores.
 * @return true when the failure line was transmitted; false for invalid input or
 * console transmission failure.
 * @post The run is latched as failed, so a PASS summary cannot subsequently be emitted.
 */
bool result_report_failure(const char *reason);

/**
 * @brief Prints the final summary, allowing PASS only for a complete successful run.
 *
 * @param[in] passed Number of reference vectors that passed comparison.
 * @param[in] total Number of reference vectors executed or expected in this run.
 * @param[in] cleanup_succeeded Whether model/runtime cleanup completed successfully.
 * @return true when the summary line was transmitted; false when the summary is
 * invalid for PASS or the console transmission fails.
 * @pre All vector processing and runtime cleanup have completed.
 * @note PASS is emitted only for five passed comparisons, five total vectors,
 * successful cleanup, successful startup/readiness, and no earlier failure.
 */
bool result_report_summary(uint32_t passed,
                           uint32_t total,
                           bool cleanup_succeeded);

/**
 * @brief Latches an additional test failure without emitting a separate line.
 *
 * @param[in] reason Identifier using only letters, digits, and underscores.
 * @return true when the reason is valid and the failure state was latched.
 * @post Any later summary is forced to FAIL.
 */
bool result_report_latch_failure(const char *reason);

#endif /* RESULT_REPORT_H */
