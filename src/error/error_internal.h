/**
 * \file error_internal.h
 * \brief Internal helpers of the error codes.
 *
 * \par Example
 * \code
 * if (nbytes > buffer->size) {
 *     vmnl_net_error_set(error, VMNL_NET_EINVAL);
 *     return false;
 * }
 * \endcode
 */

#ifndef VMNL_NET_ERROR_INTERNAL_H
#define VMNL_NET_ERROR_INTERNAL_H

#include <vmnl/net/error.h> /* IWYU pragma: export */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Writes an error code if the caller asked for one.
 * \param error Error pointer of the caller, nullable.
 * \param value Code to write.
 * \remark A NULL \p error is skipped, so public functions call it on every return path, #VMNL_NET_SUCCESS included, without checking.
 */
static inline void vmnl_net_error_set(VmnlNetError *error, VmnlNetError value)
{
    if (error == NULL) {
        return;
    }
    *error = value;
}

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ERROR_INTERNAL_H */
