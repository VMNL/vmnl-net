/**
 * \file clock.h
 *
 * \defgroup Clock Clock
 * \addtogroup Clock
 * \brief Monotonic clock.
 *
 * The clock never goes back, even when the system time changes, so the difference between two readings is always a valid duration.
 * It reads CLOCK_BOOTTIME on Linux, CLOCK_MONOTONIC on macOS and QueryPerformanceCounter() on Windows, which all count the time spent asleep: a timeout expires on waking up if its time has passed.
 * Durations are written with #VMNL_NET_NANOSECONDS_PER_MILLISECOND or #VMNL_NET_NANOSECONDS_PER_SECOND, and 64 bits hold 584 years of nanoseconds.
 * @{
 */

#ifndef VMNL_NET_CLOCK_H
#define VMNL_NET_CLOCK_H

#include <vmnl/net_export.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VMNL_NET_NANOSECONDS_PER_MILLISECOND ((uint64_t) 1000000) /*!< Nanoseconds in one millisecond. */

#define VMNL_NET_NANOSECONDS_PER_SECOND ((uint64_t) 1000000000) /*!< Nanoseconds in one second. */

/**
 * \brief Gets the time of a monotonic clock, in nanoseconds.
 * \return Nanoseconds elapsed since an unspecified point, only meaningful as a difference between two calls.
 * \note The clock keeps counting while the system is asleep.
 */
VMNL_NET_API uint64_t vmnl_net_clock_now(void);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CLOCK_H */

/** @} */
