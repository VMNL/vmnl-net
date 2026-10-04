/**
 * \file context.h
 *
 * \defgroup Context Context
 * \addtogroup Context
 * \brief Root object of vmnl_net.
 * @{
 */

#ifndef VMNL_NET_CONTEXT_H
#define VMNL_NET_CONTEXT_H

#include <vmnl/net/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque context of the library.
 */
typedef struct VmnlNetContext VmnlNetContext;

/**
 * \brief Creates a context.
 * \param context Receives the new context, or NULL on failure.
 * \return VMNL_NET_SUCCESS, VMNL_NET_EINVAL if *context* is NULL, or VMNL_NET_ENOMEM.
 */
VMNL_NET_API VmnlNetError vmnl_net_context_create(VmnlNetContext **context);

/**
 * \brief Destroys a context.
 * \param context Context to destroy, NULL does nothing.
 */
VMNL_NET_API void vmnl_net_context_destroy(VmnlNetContext *context);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_H */

/** @} */
