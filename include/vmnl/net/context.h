/**
 * \file context.h
 *
 * \defgroup Context Context
 * \addtogroup Context
 * \brief Per-thread state, such as the timers.
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
 * \attention A context **MUST** be used by one thread at a time, as it has no lock.
 */
typedef struct VmnlNetContext VmnlNetContext;

/**
 * \brief Creates a context.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created context on success, NULL otherwise.
 */
VMNL_NET_API VMNL_NET_NODISCARD VmnlNetContext *vmnl_net_context_create(VmnlNetError *error);

/**
 * \brief Gets the time left before the next deadline of the active timers of a context, to use as the timeout of a wait.
 * \param context Context of the timers.
 * \param now Current time, see vmnl_net_clock_now().
 * \return Number of nanoseconds left, 0 if the deadline passed, UINT64_MAX if no timer is active.
 */
VMNL_NET_API uint64_t vmnl_net_context_remaining(const VmnlNetContext *context, uint64_t now);

/**
 * \brief Calls the callbacks of the expired timers of a context.
 * \param context Context of the timers.
 * \param now Current time, see vmnl_net_clock_now().
 * \remark Calls the timers in the order of their deadlines, once each, even a periodic timer that missed several deadlines.
 * A periodic timer is then rearmed, unless its callback stopped, started or destroyed it.
 * A timer started by a callback with a deadline already passed waits for the next call, so a callback cannot keep this call running.
 */
VMNL_NET_API void vmnl_net_context_advance(VmnlNetContext *context, uint64_t now);

/**
 * \brief Destroys a context.
 * \param context Context to destroy, nullable.
 * \remark Stops the timers still active in it. They stay valid and can be started in another context.
 */
VMNL_NET_API void vmnl_net_context_destroy(VmnlNetContext *context);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_H */

/** @} */
