/**
 * \file context.h
 *
 * \defgroup Context Context
 * \addtogroup Context
 * \brief Context of the library.
 *
 * A context holds the state shared by the objects of one thread, such as its timers.
 *
 * \par Example
 * Example to be written.
 * @{
 */

#ifndef VMNL_NET_CONTEXT_H
#define VMNL_NET_CONTEXT_H

#include <vmnl/net/attribute.h>
#include <vmnl/net/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque context.
 * \attention A context **MUST** be used by one thread at a time: it has no lock, so each thread gets its own.
 */
typedef struct VmnlNetContext VmnlNetContext;

/**
 * \brief Creates a context.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created context on success, NULL otherwise.
 * \remark The context is allocated with the global allocator, see vmnl_net_allocator_set().
 */
VMNL_NET_API VMNL_NET_NODISCARD VmnlNetContext *vmnl_net_context_create(VmnlNetError *error);

/**
 * \brief Destroys a context.
 * \param context Context to destroy, nullable.
 */
VMNL_NET_API void vmnl_net_context_destroy(VmnlNetContext *context);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_H */

/** @} */
