/**
 * \file core.h
 *
 * \defgroup Core Core
 * \addtogroup Core
 * \brief Library's core features: version and memory allocator.
 * @{
 */

#ifndef VMNL_NET_CORE_H
#define VMNL_NET_CORE_H

#include <vmnl/net_export.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Memory allocation callbacks.
 * \note Each function **MUST** have the same contract as its standard counterpart, including the alignment of the returned memory.
 */
typedef struct VmnlNetAllocator {
    void *(*allocate)(size_t size); /*!< Equivalent of malloc(). */
    void *(*reallocate)(void *pointer, size_t size); /*!< Equivalent of realloc(). */
    void (*deallocate)(void *pointer); /*!< Equivalent of free(). */
} VmnlNetAllocator;

/**
 * \brief Gets the current library's version.
 * \return Version as (MAJOR << 16) | (MINOR << 8) | PATCH.
 */
VMNL_NET_API uint32_t vmnl_net_version(void);

/**
 * \brief Gets the current library's version as a string.
 * \return Static string as "MAJOR.MINOR.PATCH".
 */
VMNL_NET_API const char *vmnl_net_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CORE_H */

/** @} */
