/**
 * \file context.h
 *
 * \defgroup Context Context
 * \addtogroup Context
 * \brief Library's context - WIP.
 * @{
 */

#ifndef VMNL_NET_CONTEXT_H
#define VMNL_NET_CONTEXT_H

#include <vmnl/net/core.h>
#include <vmnl/net/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque handle to a context.
 */
typedef struct VmnlNetContext VmnlNetContext;

/**
 * \brief Creates a context backed by malloc(), realloc() and free().
 * \param error Error code of the call, nullable:
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created context on success, NULL otherwise.
 */
VMNL_NET_API VmnlNetContext *vmnl_net_context_create(VmnlNetError *error);

/**
 * \brief Creates a context backed by a custom allocator.
 * \param allocator Functions used to allocate/deallocate memory, copied into the context.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *allocator* or one of its functions is NULL.
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created context on success, NULL otherwise.
 * \note These functions allocate the memory of the context and of every object created from it.
 */
VMNL_NET_API VmnlNetContext *vmnl_net_context_create_with_allocator(const VmnlNetAllocator *allocator, VmnlNetError *error);

/**
 * \brief Destroys a context.
 * \param context Pointer to the VmnlNetContext to destroy.
 * \note Passing a NULL pointer is a no-op.
 */
VMNL_NET_API void vmnl_net_context_destroy(VmnlNetContext *context);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_H */

/** @} */
