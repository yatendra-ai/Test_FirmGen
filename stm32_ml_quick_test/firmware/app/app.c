/*
 * @file app.c
 * @brief Runs the fixed five-vector ST AI test and reports deterministic status.
 *
 * The reference sequence is documented in c5_vectors.h and always runs in this
 * order: positive, first_clamped, second_clamped, zero_input, both_clamped.
 * Comparisons use the original FP32 values before display conversion.
 */
#include "app.h"

#include <math.h>
#include <stdint.h>

#include "app_config.h"
#include "c5_ml_quick_test.h"
#include "c5_vectors.h"
#include "model_identity.h"
#include "result_report.h"
#include "vector_compare.h"

#define APP_TEST_STATUS_NOT_STARTED 0U
#define APP_TEST_STATUS_PASS 1U
#define APP_TEST_STATUS_FAIL 2U
#define APP_VECTOR_RESULT_NOT_RUN 0U
#define APP_VECTOR_RESULT_PASS 1U
#define APP_VECTOR_RESULT_FAIL 2U
#define APP_VECTOR_ABSOLUTE_TOLERANCE 0.000001F
#define APP_DISPLAY_INVALID_MILLI INT32_MIN

_Static_assert(APP_TEST_VECTOR_COUNT == C5_REFERENCE_VECTOR_COUNT,
               "Application and reference-vector counts must match");
_Static_assert(APP_TEST_VECTOR_COUNT == 5U, "The approved test requires five vectors");

typedef struct
{
    uint32_t status;
    uint32_t vectors_executed;
    uint32_t vectors_passed;
    uint32_t comparisons[APP_TEST_VECTOR_COUNT];
} app_test_state_t;

static app_test_state_t s_test_state;

/**
 * @brief Resets all test counters and comparison result slots before execution.
 */
static void app_test_state_initialize(void)
{
    uint32_t vector_index;

    s_test_state.status = APP_TEST_STATUS_NOT_STARTED;
    s_test_state.vectors_executed = 0U;
    s_test_state.vectors_passed = 0U;

    for (vector_index = 0U; vector_index < APP_TEST_VECTOR_COUNT; ++vector_index)
    {
        s_test_state.comparisons[vector_index] = APP_VECTOR_RESULT_NOT_RUN;
    }
}

/**
 * @brief Converts a runtime/inference outcome to a deterministic failure summary.
 *
 * @param[in] reason Stable failure identifier.
 * @param[in] runtime_initialized Whether model runtime teardown must be attempted.
 * @return true only if the failure message and summary were transmitted.
 */
static bool app_report_failure(const char *reason, bool runtime_initialized)
{
    bool cleanup_succeeded = true;

    s_test_state.status = APP_TEST_STATUS_FAIL;
    (void)result_report_latch_failure(reason);
    if (runtime_initialized)
    {
        cleanup_succeeded = c5_ml_runtime_deinit();
        if (!cleanup_succeeded)
        {
            (void)result_report_latch_failure("ml_runtime_deinit_failed");
        }
    }

    (void)result_report_failure(reason);
    (void)result_report_summary(s_test_state.vectors_passed,
                                s_test_state.vectors_executed,
                                cleanup_succeeded);
    return false;
}

/**
 * @brief Initializes the model and executes all reference comparisons in order.
 */
void app_start(void)
{
    uint32_t vector_index;
    bool cleanup_succeeded;
    int32_t output_0_milli;
    int32_t output_1_milli;

    app_test_state_initialize();
    result_report_reset();

    if (!result_report_startup(APP_MODEL_SHA256_HEX))
    {
        s_test_state.status = APP_TEST_STATUS_FAIL;
        return;
    }

    if (!c5_ml_runtime_init())
    {
        (void)app_report_failure("ml_runtime_init_failed", false);
        return;
    }

    if (!result_report_runtime_ready())
    {
        (void)app_report_failure("console_runtime_ready_failed", true);
        return;
    }

    for (vector_index = 0U; vector_index < APP_TEST_VECTOR_COUNT; ++vector_index)
    {
        const c5_reference_vector_t *vector = &C5_REFERENCE_VECTORS[vector_index];
        float output[2] = {NAN, NAN};
        bool output_valid;
        bool comparison_0_passed;
        bool comparison_1_passed;
        bool vector_passed;

        output_0_milli = APP_DISPLAY_INVALID_MILLI;
        output_1_milli = APP_DISPLAY_INVALID_MILLI;
        ++s_test_state.vectors_executed;
        if (!c5_ml_infer(vector->input, output))
        {
            s_test_state.comparisons[vector_index] = APP_VECTOR_RESULT_FAIL;
            (void)result_report_latch_failure("ml_inference_failed");
            (void)result_report_failure("ml_inference_failed");
            cleanup_succeeded = c5_ml_runtime_deinit();
            if (!cleanup_succeeded)
            {
                (void)result_report_latch_failure("ml_runtime_deinit_failed");
            }
            s_test_state.status = APP_TEST_STATUS_FAIL;
            (void)result_report_summary(s_test_state.vectors_passed,
                                        s_test_state.vectors_executed,
                                        cleanup_succeeded);
            return;
        }

        output_valid = isfinite(output[0]) && isfinite(output[1]);
        comparison_0_passed = output_valid &&
            vector_compare_fp32(output[0], vector->expected[0], APP_VECTOR_ABSOLUTE_TOLERANCE);
        comparison_1_passed = output_valid &&
            vector_compare_fp32(output[1], vector->expected[1], APP_VECTOR_ABSOLUTE_TOLERANCE);
        vector_passed = comparison_0_passed && comparison_1_passed;

        /* Both original FP32 comparisons are complete before display conversion. */
        if (output_valid &&
            vector_output_to_milli(output[0], &output_0_milli) &&
            vector_output_to_milli(output[1], &output_1_milli))
        {
            /* Integer formatting is performed only after FP32 comparison. */
        }
        else
        {
            vector_passed = false;
            (void)result_report_latch_failure("ml_output_invalid_or_unrepresentable");
        }

        s_test_state.comparisons[vector_index] = vector_passed
            ? APP_VECTOR_RESULT_PASS
            : APP_VECTOR_RESULT_FAIL;
        if (vector_passed)
        {
            ++s_test_state.vectors_passed;
        }
        else
        {
            (void)result_report_latch_failure("ml_vector_comparison_failed");
        }

        if (!result_report_vector(vector->id,
                                  output_0_milli,
                                  output_1_milli,
                                  vector_passed))
        {
            s_test_state.status = APP_TEST_STATUS_FAIL;
            cleanup_succeeded = c5_ml_runtime_deinit();
            (void)result_report_latch_failure("console_vector_transmit_failed");
            (void)result_report_failure("console_vector_transmit_failed");
            (void)result_report_summary(s_test_state.vectors_passed,
                                        s_test_state.vectors_executed,
                                        cleanup_succeeded);
            return;
        }
    }

    cleanup_succeeded = c5_ml_runtime_deinit();
    if (!cleanup_succeeded)
    {
        s_test_state.status = APP_TEST_STATUS_FAIL;
        (void)result_report_latch_failure("ml_runtime_deinit_failed");
        (void)result_report_failure("ml_runtime_deinit_failed");
        (void)result_report_summary(s_test_state.vectors_passed,
                                    s_test_state.vectors_executed,
                                    false);
        return;
    }

    if ((s_test_state.vectors_executed == APP_TEST_VECTOR_COUNT) &&
        (s_test_state.vectors_passed == APP_TEST_VECTOR_COUNT))
    {
        s_test_state.status = APP_TEST_STATUS_PASS;
    }
    else
    {
        s_test_state.status = APP_TEST_STATUS_FAIL;
    }

    (void)result_report_summary(s_test_state.vectors_passed,
                                s_test_state.vectors_executed,
                                cleanup_succeeded);
}
