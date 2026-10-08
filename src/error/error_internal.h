/**
 * \file error_internal.h
 * \brief Internal helpers for error codes.
 */

#ifndef VMNL_NET_ERROR_INTERNAL_H
#define VMNL_NET_ERROR_INTERNAL_H

#include <vmnl/net/error.h> /* IWYU pragma: export */

#include <stddef.h>

/**
 * \brief Sets the error code.
 * \param error Error pointer, nullable.
 * \param value Error code to set.
 */
static inline void vmnl_net_error_set(VmnlNetError *error, VmnlNetError value)
{
    if (error == NULL) {
        return;
    }
    *error = value;
}

#endif /* !VMNL_NET_ERROR_INTERNAL_H */
