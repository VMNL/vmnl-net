/**
 * \file timer.h
 *
 * \defgroup Timer Timer
 * \addtogroup Timer
 * \brief Application-driven timers.
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
 * \brief Opaque handle to a timer.
 * \warning Functions taking a context **MUST** receive the one that created the timer.
 */
typedef struct VmnlNetTimer VmnlNetTimer;

/**
 * \brief Function called when a timer expires.
 * \param timer Expired timer.
 * \param user_data Pointer given to vmnl_net_timer_create(), nullable.
 * \note It can start, reset, stop or destroy any timer, its own included.
 */
typedef void (*VmnlNetTimerCallback)(VmnlNetTimer *timer, void *user_data);

/**
 * \brief Creates a stopped timer.
 * \param context Context allocating the timer.
 * \param callback Function called at each expiration.
 * \param user_data Pointer given to *callback*, nullable.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: *context* or *callback* is NULL.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created timer on success, NULL otherwise.
 */
VMNL_NET_API VmnlNetTimer *vmnl_net_timer_create(VmnlNetContext *context, VmnlNetTimerCallback callback, void *user_data, VmnlNetError *error);

/**
 * \brief Stops and destroys a timer.
 * \param context Context that created *timer*.
 * \param timer Timer to destroy.
 * \note Does nothing if *context* or *timer* is NULL.
 */
VMNL_NET_API void vmnl_net_timer_destroy(VmnlNetContext *context, VmnlNetTimer *timer);

/**
 * \brief Starts a timer, or restarts it if it runs.
 * \param context Context that created *timer*.
 * \param timer Timer to start.
 * \param now Current time in nanoseconds.
 * \param timeout Nanoseconds before the first expiration.
 * \param interval Nanoseconds between the next expirations, 0 for a one-shot timer.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: *context* or *timer* is NULL, or *now* + *timeout* overflows.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_timer_start(VmnlNetContext *context, VmnlNetTimer *timer, uint64_t now, uint64_t timeout, uint64_t interval, VmnlNetError *error);

/**
 * \brief Restarts a timer from *now* with its last timeout and interval.
 * \param context Context that created *timer*.
 * \param timer Timer to restart, running or stopped.
 * \param now Current time in nanoseconds.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: *context* or *timer* is NULL, *timer* was never started, or *now* + timeout overflows.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_timer_reset(VmnlNetContext *context, VmnlNetTimer *timer, uint64_t now, VmnlNetError *error);

/**
 * \brief Stops a timer.
 * \param context Context that created *timer*.
 * \param timer Timer to stop.
 * \note Does nothing if *context* or *timer* is NULL, or if *timer* is stopped.
 */
VMNL_NET_API void vmnl_net_timer_stop(VmnlNetContext *context, VmnlNetTimer *timer);

/**
 * \brief Calls the callbacks of the expired timers.
 * \param context Context of the timers.
 * \param now Current time in nanoseconds.
 * \note A periodic timer expires once per call, even if it missed several periods.
 * \note A timer started from a callback expires at the next call at the earliest.
 * \note Does nothing if *context* is NULL.
 */
VMNL_NET_API void vmnl_net_timer_update(VmnlNetContext *context, uint64_t now);

/**
 * \brief Gets the time left before the next expiration.
 * \param context Context of the timers.
 * \param now Current time in nanoseconds.
 * \return Nanoseconds before the next expiration, 0 if a timer expired, UINT64_MAX if no timer runs or *context* is NULL.
 */
VMNL_NET_API uint64_t vmnl_net_timer_remaining(const VmnlNetContext *context, uint64_t now);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_TIMER_H */

/** @} */
