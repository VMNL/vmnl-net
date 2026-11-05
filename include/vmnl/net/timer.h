/**
 * \file timer.h
 *
 * \defgroup Timer Timer
 * \addtogroup Timer
 * \brief One-shot and periodic timers, driven by the time the application gives.
 *
 * The application gives the current time to the context of the timers with vmnl_net_context_advance(), which calls the expired ones.
 *
 * \par Deadlines
 * - A timeout counts from the last advance of the context, or from its creation before the first one.
 * - A timer expires at the first advance whose time reaches its deadline, so late but never early.
 * - A late periodic timer is called once, with its lateness, and its next deadlines do not shift.
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
 * \attention A timer **MUST** be used by the thread of the context it is started in, as they share no lock.
 */
typedef struct VmnlNetTimer VmnlNetTimer;

/**
 * \brief Function called when a timer expires.
 * \param timer Expired timer.
 * \param lateness Nanoseconds since the deadline. A periodic timer also missed \p lateness / interval deadlines.
 * \param data Pointer given with the callback, nullable.
 * \remark It can start, stop, change or destroy any timer, its own included.
 */
typedef void (*VmnlNetTimerCallback)(VmnlNetTimer *timer, uint64_t lateness, void *data);

/**
 * \brief Creates an inactive one-shot timer.
 * \param callback Function called at each expiration.
 * \param data Pointer given to \p callback, nullable, never freed by the library.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created timer on success, NULL otherwise.
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
 * \param now Current time, see vmnl_net_clock_now().
 * \return Nanoseconds before the next deadline of \p timer, 0 if it passed.
 * \attention \p timer **MUST** be active, as an inactive timer has no deadline.
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
 * \brief Sets the interval of a periodic timer.
 * \param timer Timer to change.
 * \param interval Nanoseconds between the deadlines after the next one, 0 for a one-shot timer, the default.
 */
VMNL_NET_API void vmnl_net_timer_set_interval(VmnlNetTimer *timer, uint64_t interval);

/**
 * \brief Starts a timer, or restarts it if it is active.
 * \param context Context that calls the timer, see vmnl_net_context_advance().
 * \param timer Timer to start.
 * \param timeout Nanoseconds before the first deadline, 0 for the next advance, the deadline stopping at UINT64_MAX.
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
