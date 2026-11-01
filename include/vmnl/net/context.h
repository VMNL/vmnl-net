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

#include <stdbool.h>
#include <stdint.h>

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
 * \brief Gets the time left before the first deadline of the timers of a context.
 * \param context Context of the timers.
 * \param now Current time, see vmnl_net_clock_now().
 * \param remaining Time left before the first deadline, 0 if it passed, left unchanged when no timer is active.
 * \return true if a timer is active in \p context, false otherwise.
 * \remark Meant as the timeout of the wait of the loop, so that it wakes up for the first deadline.
 */
VMNL_NET_API bool vmnl_net_context_remaining(const VmnlNetContext *context, uint64_t now, uint64_t *remaining);

/**
 * \brief Calls the callbacks of the timers whose deadline \p now reached.
 * \param context Context of the timers.
 * \param now Current time, see vmnl_net_clock_now(), from which the timers of \p context start until the next call.
 * \remark Calls them in the order of their deadlines, once per timer, even for a periodic timer that missed deadlines.
 * \remark A timer started from a callback with a deadline already reached expires at the next call, so a callback cannot keep this call running.
 * \remark A periodic timer is rearmed after its callback, unless the callback stopped, started or destroyed it.
 */
VMNL_NET_API void vmnl_net_context_advance(VmnlNetContext *context, uint64_t now);

/**
 * \brief Destroys a context.
 * \param context Context to destroy, nullable.
 * \remark Stops the timers active in it: they stay valid and can be started in another context.
 */
VMNL_NET_API void vmnl_net_context_destroy(VmnlNetContext *context);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_H */

/** @} */
