/**
 * \file timer_internal.h
 * \brief Internal definition and helpers of the timers.
 */

#ifndef VMNL_NET_TIMER_INTERNAL_H
#define VMNL_NET_TIMER_INTERNAL_H

#include <vmnl/net/timer.h> /* IWYU pragma: export */

#include "core/list_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Internal definition of a timer.
 */
struct VmnlNetTimer {
    VmnlNetList node; /*!< Link in the running timers of the context, to itself while stopped. */
    VmnlNetTimerCallback callback; /*!< Function called at each expiration. */
    void *user_data; /*!< Pointer given to *callback*. */
    uint64_t deadline; /*!< Time of the next expiration, in nanoseconds. */
    uint64_t timeout; /*!< Last timeout given to vmnl_net_timer_start(), reused by vmnl_net_timer_reset(). */
    uint64_t interval; /*!< Period in nanoseconds, 0 for a one-shot timer. */
};

/**
 * \brief Gets the first timer of a list.
 * \param list List of timers.
 * \return First timer, NULL if *list* is empty.
 */
static inline VmnlNetTimer *vmnl_net_timer_first(const VmnlNetList *list)
{
    return vmnl_net_list_empty(list) ? NULL : VMNL_NET_LIST_ENTRY(list->next, VmnlNetTimer, node);
}

/**
 * \brief Adds a delay to a time without going past UINT64_MAX.
 * \param time Time in nanoseconds.
 * \param delay Nanoseconds to add.
 * \return *time* + *delay*, or UINT64_MAX if the sum does not fit.
 */
static inline uint64_t vmnl_net_timer_add(uint64_t time, uint64_t delay)
{
    return (delay > UINT64_MAX - time) ? UINT64_MAX : time + delay;
}

/**
 * \brief Adds a timer to the running timers of a context, sorted by deadline.
 * \param context Context that created *timer*.
 * \param timer Timer to add, **MUST** not be running.
 * \note Timers with the same deadline keep the order in which they were added.
 */
void vmnl_net_timer_insert(VmnlNetContext *context, VmnlNetTimer *timer);

/**
 * \brief Removes a timer from the running timers of its context.
 * \param timer Timer to remove, does nothing if it is stopped.
 */
void vmnl_net_timer_remove(VmnlNetTimer *timer);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_TIMER_INTERNAL_H */
