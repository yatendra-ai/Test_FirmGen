/*
 * @file vector_compare.c
 * @brief Pure FP32 absolute-tolerance comparison and checked milli-unit conversion.
 */
#include "vector_compare.h"

#include <limits.h>
#include <math.h>

#define VECTOR_DISPLAY_SCALE 1000.0

bool vector_compare_fp32(float actual, float expected, float absolute_tolerance)
{
    if (!isfinite(actual) || !isfinite(expected) ||
        !isfinite(absolute_tolerance) || (absolute_tolerance < 0.0F))
    {
        return false;
    }

    return fabsf(actual - expected) <= absolute_tolerance;
}

bool vector_output_to_milli(float value, int32_t *milli_units)
{
    const double scaled_value = (double)value * VECTOR_DISPLAY_SCALE;

    if ((milli_units == NULL) || !isfinite(value) || !isfinite(scaled_value) ||
        (scaled_value > (double)INT32_MAX) || (scaled_value < (double)INT32_MIN))
    {
        return false;
    }

    *milli_units = (int32_t)scaled_value;
    return true;
}
