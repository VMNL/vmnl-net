/**
 * \file net.h
 *
 * \defgroup Net Network
 * \addtogroup Net
 * \brief Public API of vmnl_net.
 * @{
 */

#ifndef VMNL_NET_H
#define VMNL_NET_H

#include <stdint.h>

#include "vmnl/net_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Gets the version of the library as an unsigned 32-bit integer.
 * \return The version encoded as `(MAJOR << 16) | (MINOR << 8) | PATCH`, so that two versions compare with the usual operators.
 */
VMNL_NET_API uint32_t vmnl_net_version(void);

/**
 * \brief Gets the version of the library as a string.
 * \return The version as a static "MAJOR.MINOR.PATCH" string, never NULL.
 */
VMNL_NET_API const char *vmnl_net_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_H */

/** @} */
