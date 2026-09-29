/*
 * @file console_if.h
 * @brief Vendor-neutral synchronous console transmission contract.
 */
#ifndef CONSOLE_IF_H
#define CONSOLE_IF_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Transmits a complete byte buffer through the configured console.
 *
 * @param[in] data Pointer to the bytes to transmit; must not be NULL when length is nonzero.
 * @param[in] length Number of bytes to transmit.
 * @return true when the platform confirms the complete transmission; false on invalid
 * arguments, an unavailable transport, or a transport error.
 * @pre The platform console peripheral has been initialized.
 * @note The operation is synchronous and may block for the platform's bounded timeout.
 */
bool console_if_write(const uint8_t *data, size_t length);

#endif /* CONSOLE_IF_H */
