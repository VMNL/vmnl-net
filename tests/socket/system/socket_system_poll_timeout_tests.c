#include "socket/system/socket_system_internal.h"

#include "utest.h"

#define MILLISECOND (VMNL_NET_NANOSECONDS_PER_SECOND / VMNL_NET_MILLISECONDS_PER_SECOND) /*!< Nanoseconds in a millisecond. */

UTEST(vmnl_net_socket_system_poll_timeout, returns_time_left)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_timeout(5 * MILLISECOND, 2 * MILLISECOND), 3);
}

UTEST(vmnl_net_socket_system_poll_timeout, rounds_partial_millisecond_up)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_timeout(MILLISECOND + 1, 0), 2);
}

UTEST(vmnl_net_socket_system_poll_timeout, returns_zero_when_time_is_over)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_timeout(MILLISECOND, 2 * MILLISECOND), 0);
}

UTEST(vmnl_net_socket_system_poll_timeout, returns_minus_one_without_limit)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_timeout(VMNL_NET_CLOCK_INF, 0), -1);
}

UTEST(vmnl_net_socket_system_poll_timeout, caps_at_int_max)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_timeout(((uint64_t) INT_MAX + 1) * MILLISECOND, 0), INT_MAX);
}
