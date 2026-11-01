/**
 * \addtogroup Timer
 * @{
 */

#ifndef VMNL_NET_TIMER_INTERNAL_H
#define VMNL_NET_TIMER_INTERNAL_H

#include <vmnl/net/timer.h> /* IWYU pragma: export */

#include "list/list_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Timer, opaque outside the library.
 * \remark Active while #link is in a list, see vmnl_net_list_is_empty().
 * \remark While its periodic callback runs, vmnl_net_context_advance() holds the timer alone in a local list, the honeypot: vmnl_net_timer_stop(), vmnl_net_timer_start() and vmnl_net_timer_destroy() all take it out, so an empty honeypot after the callback tells that the callback took the timer over, and the timer, maybe freed, is not read again.
 */
struct VmnlNetTimer {
    struct VmnlNetListLink link; /*!< Link in the active timers of the context it was started in, sorted by deadline, to itself while inactive. */
    VmnlNetTimerCallback callback; /*!< Function called at each expiration. */
    void *data; /*!< Pointer given to #callback, nullable. */
    uint64_t deadline; /*!< Time of the next expiration, in nanoseconds of vmnl_net_clock_now(), meaningful while active. */
    uint64_t interval; /*!< Nanoseconds between two deadlines, 0 for a one-shot timer. */
};

/**
 * \brief Computes the deadline a delay after a time, without wrapping around.
 * \param time Time in nanoseconds of vmnl_net_clock_now().
 * \param delay Nanoseconds after \p time.
 * \return \p time + \p delay, UINT64_MAX if the sum does not fit.
 */
static inline uint64_t vmnl_net_timer_compute_deadline(uint64_t time, uint64_t delay)
{
    return (delay > UINT64_MAX - time) ? UINT64_MAX : time + delay;
}

/**
 * \brief Inserts a timer in a list of active timers, sorted by deadline.
 * \param active_timers Head of the list.
 * \param timer Timer to insert.
 * \attention \p timer **MUST** be inactive: inserting a link that is in a list would corrupt both lists.
 * \remark Searches from the back for the last timer whose deadline is not later: O(1) when the timeouts are equal, and timers with the same deadline keep the order of their start.
 */
static inline void vmnl_net_timer_insert(struct VmnlNetListLink *active_timers, VmnlNetTimer *timer)
{
    VmnlNetTimer *other;

    VMNL_NET_LIST_FOREACH_REVERSE(other, active_timers, link)
    {
        if (other->deadline <= timer->deadline) {
            vmnl_net_list_insert_after(&other->link, &timer->link);
            return;
        }
    }
    vmnl_net_list_push_front(active_timers, &timer->link);
}

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_TIMER_INTERNAL_H */

/** @} */
