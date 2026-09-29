/*
 * @file result_report.c
 * @brief Formats deterministic startup, inference, vector, and summary messages.
 *
 * The reporter owns per-run readiness/failure state and uses a vendor-neutral
 * console function. PASS is guarded by completed vector and cleanup evidence.
 */
#include "result_report.h"

#include <stdio.h>
#include <string.h>

#include "console_if.h"

#define RESULT_REPORT_LINE_CAPACITY 192U
#define RESULT_REPORT_VECTOR_TOTAL 5U
#define RESULT_REPORT_SHA256_LENGTH 64U

typedef struct
{
    bool startup_sent;
    bool runtime_ready_sent;
    bool failed;
    uint32_t vector_lines_sent;
    uint32_t vector_passes;
} result_report_state_t;

static result_report_state_t s_report_state;

/**
 * @brief Checks an exact 64-character hexadecimal model digest.
 *
 * @param[in] digest Candidate digest string.
 * @return true only for 64 hexadecimal characters followed by a null byte.
 */
static bool result_report_digest_is_valid(const char *digest)
{
    size_t index;

    if (digest == NULL)
    {
        return false;
    }

    for (index = 0U; index < RESULT_REPORT_SHA256_LENGTH; ++index)
    {
        const char character = digest[index];
        const bool is_decimal = (character >= '0') && (character <= '9');
        const bool is_lower_hex = (character >= 'a') && (character <= 'f');
        const bool is_upper_hex = (character >= 'A') && (character <= 'F');

        if (!(is_decimal || is_lower_hex || is_upper_hex))
        {
            return false;
        }
    }

    return digest[RESULT_REPORT_SHA256_LENGTH] == '\0';
}

/**
 * @brief Validates a restricted token alphabet to keep log lines parseable.
 *
 * @param[in] identifier Candidate identifier.
 * @return true for a non-empty sequence of letters, digits, or underscores.
 */
static bool result_report_identifier_is_valid(const char *identifier)
{
    size_t index;

    if ((identifier == NULL) || (identifier[0] == '\0'))
    {
        return false;
    }

    for (index = 0U; identifier[index] != '\0'; ++index)
    {
        const char character = identifier[index];
        const bool is_lower = (character >= 'a') && (character <= 'z');
        const bool is_upper = (character >= 'A') && (character <= 'Z');
        const bool is_decimal = (character >= '0') && (character <= '9');

        if (!(is_lower || is_upper || is_decimal || (character == '_')))
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Sends one bounded line and latches failure on a console error.
 *
 * @param[in] line Buffer containing a line without its newline.
 * @param[in] line_length Number of formatted bytes in line.
 * @return true if all line bytes and the newline were accepted by the console.
 */
static bool result_report_send_line(const char *line, size_t line_length)
{
    uint8_t terminated_line[RESULT_REPORT_LINE_CAPACITY];

    if ((line == NULL) || (line_length == 0U) || (line_length >= sizeof(terminated_line)))
    {
        s_report_state.failed = true;
        return false;
    }

    (void)memcpy(terminated_line, line, line_length);
    terminated_line[line_length] = (uint8_t)'\n';

    if (!console_if_write(terminated_line, line_length + 1U))
    {
        s_report_state.failed = true;
        return false;
    }

    return true;
}

/**
 * @brief Latches an error token and attempts to report it over the console.
 *
 * @param[in] reason Stable identifier using letters, digits, and underscores.
 * @return true when the failure line was transmitted successfully.
 */
static bool result_report_invalid_input(const char *reason)
{
    char line[RESULT_REPORT_LINE_CAPACITY];
    const int line_length = snprintf(line, sizeof(line), "ML_FAIL reason=%s", reason);

    s_report_state.failed = true;
    if ((line_length < 0) || ((size_t)line_length >= sizeof(line)))
    {
        return false;
    }

    return result_report_send_line(line, (size_t)line_length);
}

void result_report_reset(void)
{
    (void)memset(&s_report_state, 0, sizeof(s_report_state));
}

bool result_report_startup(const char *sha256_hex)
{
    char line[RESULT_REPORT_LINE_CAPACITY];
    int line_length;

    if (!result_report_digest_is_valid(sha256_hex))
    {
        return result_report_invalid_input("model_sha256_invalid");
    }

    line_length = snprintf(line, sizeof(line), "ML_MODEL_SHA256=%s", sha256_hex);
    if ((line_length < 0) || ((size_t)line_length >= sizeof(line)))
    {
        return result_report_invalid_input("startup_format_error");
    }

    s_report_state.startup_sent = result_report_send_line(line, (size_t)line_length);
    return s_report_state.startup_sent;
}

bool result_report_runtime_ready(void)
{
    if (!s_report_state.startup_sent || s_report_state.failed)
    {
        return result_report_invalid_input("runtime_not_initialized");
    }

    if (!result_report_send_line("ML_RUNTIME_READY", sizeof("ML_RUNTIME_READY") - 1U))
    {
        return false;
    }

    s_report_state.runtime_ready_sent = true;
    return true;
}

bool result_report_vector(const char *vector_id,
                          int32_t output_0_milli,
                          int32_t output_1_milli,
                          bool comparison_passed)
{
    char line[RESULT_REPORT_LINE_CAPACITY];
    const char *comparison_text;
    int line_length;

    if (!s_report_state.runtime_ready_sent)
    {
        return result_report_invalid_input("vector_before_runtime_ready");
    }

    if (!result_report_identifier_is_valid(vector_id) ||
        (s_report_state.vector_lines_sent >= RESULT_REPORT_VECTOR_TOTAL))
    {
        return result_report_invalid_input("vector_record_invalid");
    }

    comparison_text = comparison_passed ? "PASS" : "FAIL";
    line_length = snprintf(line,
                           sizeof(line),
                           "ML_VECTOR id=%s comparison=%s output_milli=[%ld,%ld]",
                           vector_id,
                           comparison_text,
                           (long)output_0_milli,
                           (long)output_1_milli);

    if ((line_length < 0) || ((size_t)line_length >= sizeof(line)))
    {
        return result_report_invalid_input("vector_format_error");
    }

    if (!comparison_passed)
    {
        s_report_state.failed = true;
    }

    if (!result_report_send_line(line, (size_t)line_length))
    {
        return false;
    }

    ++s_report_state.vector_lines_sent;
    if (comparison_passed)
    {
        ++s_report_state.vector_passes;
    }

    return true;
}

bool result_report_failure(const char *reason)
{
    if (!result_report_identifier_is_valid(reason))
    {
        return result_report_invalid_input("failure_reason_invalid");
    }

    return result_report_invalid_input(reason);
}

bool result_report_latch_failure(const char *reason)
{
    if (!result_report_identifier_is_valid(reason))
    {
        return false;
    }

    s_report_state.failed = true;
    return true;
}

bool result_report_summary(uint32_t passed,
                           uint32_t total,
                           bool cleanup_succeeded)
{
    char line[RESULT_REPORT_LINE_CAPACITY];
    const bool can_pass = s_report_state.startup_sent &&
                          s_report_state.runtime_ready_sent &&
                          !s_report_state.failed &&
                          cleanup_succeeded &&
                          (passed == RESULT_REPORT_VECTOR_TOTAL) &&
                          (total == RESULT_REPORT_VECTOR_TOTAL) &&
                          (s_report_state.vector_lines_sent == RESULT_REPORT_VECTOR_TOTAL) &&
                          (s_report_state.vector_passes == RESULT_REPORT_VECTOR_TOTAL);
    const int line_length = can_pass
        ? snprintf(line, sizeof(line), "ML_SUMMARY passed=5 total=5 status=PASS")
        : snprintf(line,
                   sizeof(line),
                   "ML_SUMMARY passed=%lu total=%lu status=FAIL",
                   (unsigned long)passed,
                   (unsigned long)total);

    if ((line_length < 0) || ((size_t)line_length >= sizeof(line)))
    {
        s_report_state.failed = true;
        return false;
    }

    if (!result_report_send_line(line, (size_t)line_length))
    {
        return false;
    }

    if (!can_pass)
    {
        s_report_state.failed = true;
    }

    return can_pass;
}
