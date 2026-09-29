/*
 * @file c5_vectors.h
 * @brief Fixed reference-vector sequence from the approved ML Quick Test PRD.
 */
#ifndef C5_VECTORS_H
#define C5_VECTORS_H

#include <stdint.h>

typedef struct
{
    const char *id;
    float input[2];
    float expected[2];
} c5_reference_vector_t;

#define C5_REFERENCE_VECTOR_COUNT 5U

/* Fixed order: positive, first_clamped, second_clamped, zero_input, both_clamped. */
static const c5_reference_vector_t C5_REFERENCE_VECTORS[C5_REFERENCE_VECTOR_COUNT] = {
    {"positive",      { 1.0F,  2.0F}, {0.5F, 4.0F}},
    {"first_clamped", {-2.0F,  1.0F}, {0.0F, 4.0F}},
    {"second_clamped",{ 3.0F, -1.0F}, {7.5F, 0.0F}},
    {"zero_input",    { 0.0F,  0.0F}, {0.5F, 0.0F}},
    {"both_clamped",  {-1.0F, -1.0F}, {0.0F, 0.0F}}
};

#endif /* C5_VECTORS_H */
