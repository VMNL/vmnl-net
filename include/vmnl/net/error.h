/**
 * \file error.h
 *
 * \defgroup Error Error
 * \addtogroup Error
 * \brief Library's error codes.
 * @{
 */

#ifndef VMNL_NET_ERROR_H
#define VMNL_NET_ERROR_H

#include <vmnl/net_export.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Error code set by functions that can fail.
 * \note Functions that can fail take a nullable VmnlNetError pointer as last parameter, set it on every call and also signal a failure through their return value.
 * \note E codes are named after errno and use its glibc message, F (failure) codes are specific to the library.
 * \note Each code has the same value on every platform and in every version, and the value of a removed code is never reused.
 */
typedef uint32_t VmnlNetError;

#define VMNL_NET_SUCCESS 0u /*!< Call succeeded. */
#define VMNL_NET_EINVAL  1u /*!< Invalid argument. */
#define VMNL_NET_ENOMEM  2u /*!< Not enough space/cannot allocate memory. */
#define VMNL_NET_FSYSTEM 3u /*!< An OS call failed with an error that has no dedicated code (errno, GetLastError() or WSAGetLastError()). */

/**
 * \brief Converts a VmnlNetError to a human-readable string.
 * \param error Error code to describe.
 * \return Static string describing the error, or "Unknown error" if the code is unknown.
 */
VMNL_NET_API const char *vmnl_net_error_string(VmnlNetError error);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ERROR_H */

/** @} */
