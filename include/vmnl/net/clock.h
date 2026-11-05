/**
 * \file clock.h
 *
 * \defgroup Clock Clock
 * \addtogroup Clock
 * \brief Monotonic clock in nanoseconds, for durations and timeouts.
 *
 * Times are nanoseconds in a uint64_t, from an unspecified origin.
 *
 * \par Measuring a duration
 * \snippet example_clock_elapsed.c elapsed
 *
 * \par Checking a timeout
 * \snippet example_clock_elapsed.c timeout
 *
 * \see \ref example_clock_elapsed.c
 * @{
 */

#ifndef VMNL_NET_CLOCK_H
#define VMNL_NET_CLOCK_H

#include <vmnl/net_api.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VMNL_NET_MILLISECONDS_PER_SECOND UINT64_C(1000) /*!< Milliseconds in a second. */
#define VMNL_NET_MICROSECONDS_PER_SECOND UINT64_C(1000000) /*!< Microseconds in a second. */
#define VMNL_NET_NANOSECONDS_PER_SECOND  UINT64_C(1000000000) /*!< Nanoseconds in a second. */

/**
 * \brief Gets the current time of a monotonic clock.
 * \return Nanoseconds since an unspecified origin.
 * \remark Only the difference between two readings is meaningful, and the clock never goes back, unlike the wall clock.
 * \remark Keeps counting while the machine is asleep, on Linux, macOS and Windows. Thread-safe.
 */
VMNL_NET_API uint64_t vmnl_net_clock_now(void);

/**
 * \brief Gets the time elapsed since a reading of the clock.
 * \param start Earlier result of vmnl_net_clock_now().
 * \return Nanoseconds since \p start, 0 if \p start is in the future.
 */
VMNL_NET_API uint64_t vmnl_net_clock_elapsed(uint64_t start);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CLOCK_H */

/** @} */

/**
 * \example example_clock_elapsed.c
 * Measures a duration and checks a timeout with the monotonic clock.
 */
