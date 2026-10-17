/**
 * \file clock.h
 *
 * \defgroup Clock Clock
 * \addtogroup Clock
 * \brief Monotonic clock of the library.
 *
 * vmnl_net_clock_now() counts nanoseconds from an unspecified origin, vmnl_net_clock_elapsed() measures from one of its readings, and the constants convert them, all as uint64_t.
 *
 * \par Example
 * \code
 * uint64_t start = vmnl_net_clock_now();
 *
 * do_work();
 * printf("%" PRIu64 " ms\n", vmnl_net_clock_elapsed(start) / (VMNL_NET_NANOSECONDS_PER_SECOND / VMNL_NET_MILLISECONDS_PER_SECOND));
 * \endcode
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
 * \remark Only the difference between two calls is meaningful: the clock never goes back, unlike the wall clock.
 * \remark Read with clock_gettime(), CLOCK_BOOTTIME when available and CLOCK_MONOTONIC otherwise, and with QueryPerformanceCounter() on Windows. Thread-safe.
 */
VMNL_NET_API uint64_t vmnl_net_clock_now(void);

/**
 * \brief Gets the time elapsed since a reading of the clock.
 * \param start Earlier result of vmnl_net_clock_now().
 * \return Nanoseconds since \p start.
 * \remark Saturates to 0 when \p start is in the future, instead of wrapping around to a huge duration.
 */
VMNL_NET_API uint64_t vmnl_net_clock_elapsed(uint64_t start);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CLOCK_H */

/** @} */
