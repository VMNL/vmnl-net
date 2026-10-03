/**
 * \file core.h
 *
 * \defgroup Core Core
 * \addtogroup Core
 * \brief Version and error codes of vmnl_net.
 * @{
 */

#ifndef VMNL_NET_CORE_H
#define VMNL_NET_CORE_H

#include <stdint.h>

#include "vmnl/net_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Error code returned by the library.
 * \note Values are fixed across platforms and never reused.
 * \note E codes mirror errno and its glibc message, F (failure) codes are specific to vmnl_net.
 */
typedef uint32_t VmnlNetError;

/**
 * \brief Values of VmnlNetError.
 */
enum {
    VMNL_NET_SUCCESS = 0, /*!< The call succeeded. */
    VMNL_NET_EINVAL  = 1, /*!< An argument is NULL or out of range. */
    VMNL_NET_ENOMEM  = 2, /*!< An allocation failed. */
    VMNL_NET_FSYSTEM = 3, /*!< OS error without a dedicated code. */
};

/**
 * \brief Gets a description of an error code.
 * \param error Error code to describe.
 * \return Static string, never NULL, even for unknown codes.
 */
VMNL_NET_API const char *vmnl_net_error_string(VmnlNetError error);

/**
 * \brief Gets the version of the library as an unsigned 32-bit integer.
 * \return `(MAJOR << 16) | (MINOR << 8) | PATCH`, comparable with the usual operators.
 */
VMNL_NET_API uint32_t vmnl_net_version(void);

/**
 * \brief Gets the version of the library as a string.
 * \return Static "MAJOR.MINOR.PATCH" string, never NULL.
 */
VMNL_NET_API const char *vmnl_net_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CORE_H */

/** @} */
