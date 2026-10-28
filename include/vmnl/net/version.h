/**
 * \file version.h
 *
 * \defgroup Version Version
 * \addtogroup Version
 * \brief Version of the library.
 *
 * vmnl_net_version() packs the version in one integer, (MAJOR << 16) | (MINOR << 8) | PATCH, so two versions compare with a single comparison.
 * Before 1.0.0, a new minor version may break the API.
 *
 * \par Example
 * Example to be written.
 * @{
 */

#ifndef VMNL_NET_VERSION_H
#define VMNL_NET_VERSION_H

#include <vmnl/net_api.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Gets the major number of a packed version.
 * \param version Version packed like vmnl_net_version().
 */
#define VMNL_NET_VERSION_GET_MAJOR(version) ((version) >> 16 & 0xFF)

/**
 * \brief Gets the minor number of a packed version.
 * \param version Version packed like vmnl_net_version().
 */
#define VMNL_NET_VERSION_GET_MINOR(version) ((version) >> 8 & 0xFF)

/**
 * \brief Gets the patch number of a packed version.
 * \param version Version packed like vmnl_net_version().
 */
#define VMNL_NET_VERSION_GET_PATCH(version) ((version) & 0xFF)

/**
 * \brief Gets the version of the library.
 * \return Version as (MAJOR << 16) | (MINOR << 8) | PATCH.
 * \remark Each number takes 8 bits, up to 255.
 */
VMNL_NET_API uint32_t vmnl_net_version(void);

/**
 * \brief Gets the version of the library as text.
 * \return Static string "MAJOR.MINOR.PATCH", cannot be NULL.
 */
VMNL_NET_API const char *vmnl_net_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_VERSION_H */

/** @} */
