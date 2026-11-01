/**
 * \file timer.h
 *
 * \defgroup Timer Timer
 * \addtogroup Timer
 * \brief Timers of the network loop.
 *
 * vmnl_net_context_advance(), vmnl_net_context_remaining() and vmnl_net_timer_remaining() take the current time as \p now, in nanoseconds of vmnl_net_clock_now(), so the loop reads the clock once per turn.
 * A timer starts from the \p now of the last vmnl_net_context_advance() of the context given to vmnl_net_timer_start(), or from the vmnl_net_clock_now() read by vmnl_net_context_create() before the first call.
 * It expires at the first vmnl_net_context_advance() whose \p now reaches its deadline: late, never early.
 * A periodic timer keeps its phase: a late call reports the deadlines it missed through the lateness given to the callback, without shifting the next ones.
 *
 * \par Example
 * Example to be written.
 * @{
 */

#ifndef VMNL_NET_TIMER_H
#define VMNL_NET_TIMER_H

#include <vmnl/net/context.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque timer.
 * \attention A timer **MUST** be used by the thread of the context it is started in: they share no lock.
 */
typedef struct VmnlNetTimer VmnlNetTimer;

/**
 * \brief Function called when a timer expires.
 * \param timer Expired timer.
 * \param lateness Nanoseconds between the first deadline reached and the \p now of vmnl_net_context_advance(): a periodic timer also missed \p lateness / interval deadlines, see vmnl_net_timer_interval().
 * \param data Pointer given with the callback, nullable.
 * \remark It can start, stop or destroy any timer, its own included.
 */
typedef void (*VmnlNetTimerCallback)(VmnlNetTimer *timer, uint64_t lateness, void *data);

/**
 * \brief Creates an inactive one-shot timer.
 * \param callback Function called at each expiration.
 * \param data Pointer given to \p callback, nullable.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created timer on success, NULL otherwise.
 * \remark The timer is allocated with the global allocator, see vmnl_net_allocator_set().
 */
VMNL_NET_API VMNL_NET_NODISCARD VmnlNetTimer *vmnl_net_timer_create(VmnlNetTimerCallback callback, void *data, VmnlNetError *error);

/**
 * \brief Checks whether a timer waits for a deadline.
 * \param timer Timer to check.
 * \return true if \p timer is started and not stopped, false otherwise.
 * \remark A one-shot timer becomes inactive when it expires, before its callback runs.
 */
VMNL_NET_API bool vmnl_net_timer_is_active(const VmnlNetTimer *timer);

/**
 * \brief Gets the interval of a timer.
 * \param timer Timer to read.
 * \return Nanoseconds between the deadlines of \p timer, 0 for a one-shot timer.
 */
VMNL_NET_API uint64_t vmnl_net_timer_interval(const VmnlNetTimer *timer);

/**
 * \brief Gets the time left before the next deadline of a timer.
 * \param timer Timer to read.
 * \param now Current time.
 * \return Nanoseconds before the next deadline of \p timer, 0 if it passed.
 * \attention \p timer **MUST** be active: an inactive timer has no deadline.
 */
VMNL_NET_API uint64_t vmnl_net_timer_remaining(const VmnlNetTimer *timer, uint64_t now);

/**
 * \brief Changes the callback of a timer.
 * \param timer Timer to change.
 * \param callback Function called at each expiration.
 * \param data Pointer given to \p callback, nullable.
 */
VMNL_NET_API void vmnl_net_timer_set_callback(VmnlNetTimer *timer, VmnlNetTimerCallback callback, void *data);

/**
 * \brief Sets the interval of a timer, 0 at its creation.
 * \param timer Timer to change.
 * \param interval Nanoseconds between the deadlines that follow the first one, 0 for a one-shot timer.
 * \remark The deadline already set is kept: \p interval applies from the next one.
 */
VMNL_NET_API void vmnl_net_timer_set_interval(VmnlNetTimer *timer, uint64_t interval);

/**
 * \brief Starts a timer, or starts it again if it is active.
 * \param context Context in which \p timer waits for its deadlines.
 * \param timer Timer to start.
 * \param timeout Nanoseconds before the first deadline, 0 for the next vmnl_net_context_advance().
 * \remark The next deadlines follow at the interval of \p timer, see vmnl_net_timer_set_interval().
 * \remark The deadlines stop at UINT64_MAX instead of wrapping around.
 * \remark A timer active in another context moves to \p context.
 */
VMNL_NET_API void vmnl_net_timer_start(VmnlNetContext *context, VmnlNetTimer *timer, uint64_t timeout);

/**
 * \brief Stops a timer.
 * \param timer Timer to stop.
 * \return true if \p timer was active, false if it was already inactive or had expired.
 */
VMNL_NET_API bool vmnl_net_timer_stop(VmnlNetTimer *timer);

/**
 * \brief Stops and destroys a timer.
 * \param timer Timer to destroy, nullable.
 */
VMNL_NET_API void vmnl_net_timer_destroy(VmnlNetTimer *timer);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_TIMER_H */

/** @} */
