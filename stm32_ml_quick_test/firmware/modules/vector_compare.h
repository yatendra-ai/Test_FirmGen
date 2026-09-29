/*
 * @file vector_compare.h
 * @brief Pure FP32 comparison and milli-unit conversion helpers for the test harness.
 */
#ifndef VECTOR_COMPARE_H
#define VECTOR_COMPARE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Checks one finite FP32 output against an expected value by absolute tolerance.
 *
 * @param[in] actual Original FP32 model output.
 * @param[in] expected Expected FP32 output.
 * @param[in] absolute_tolerance Maximum accepted absolute difference, in output units.
 * @return true when both values are finite and absolute error is <= tolerance.
 * @note Relative tolerance is zero; this helper performs no scaling or formatting.
 */
bool vector_compare_fp32(float actual, float expected, float absolute_tolerance);

/**
 * @brief Converts a finite FP32 output to integer milli-units without overflow.
 *
 * @param[in] value Original FP32 output.
 * @param[out] milli_units Integer output value multiplied by 1000, truncated toward zero.
 * @return true when the input is finite and its scaled value fits in int32_t.
 */
bool vector_output_to_milli(float value, int32_t *milli_units);

#endif /* VECTOR_COMPARE_H */
