/**
 * \file error_internal.h
 * \brief Internal definition of the error codes.
 */

/**
 * \addtogroup Error
 * @{
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

/**
 * \brief Translates an error code of the system into a code of the library.
 *
 * A code whose meaning depends on the call is translated by the caller instead, like the WSAEWOULDBLOCK of a connect in progress on Windows, which means #VMNL_NET_EINPROGRESS there.
 * \param error_code Code of the failed call, errno on POSIX and WSAGetLastError() on Windows.
 * \return Code of the library with the same meaning, #VMNL_NET_FSYSTEM if none matches.
 */
VmnlNetError vmnl_net_error_from_system(int error_code);

/**
 * \brief Translates the last error of the system into a code of the library.
 *
 * It reads errno on POSIX and WSAGetLastError() on Windows.
 * A call that returns its code directly, like WSAStartup(), goes through vmnl_net_error_from_system() instead.
 * \return Code of the library with the same meaning, #VMNL_NET_FSYSTEM if none matches.
 * \attention It **MUST** be called right after the failed call, as any call in between can overwrite errno or the code of WSAGetLastError().
 */
VmnlNetError vmnl_net_error_from_last_system(void);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ERROR_INTERNAL_H */

/** @} */
