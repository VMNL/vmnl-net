#include "socket/system/socket_system_internal.h"

#include "utest.h"

UTEST(vmnl_net_socket_system_poll_events, converts_readable)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_events(VMNL_NET_SOCKET_READABLE), (short) POLLIN);
}

UTEST(vmnl_net_socket_system_poll_events, converts_writable)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_events(VMNL_NET_SOCKET_WRITABLE), (short) POLLOUT);
}

UTEST(vmnl_net_socket_system_poll_events, converts_both_events)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_events(VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE), (short) ((unsigned int) POLLIN | (unsigned int) POLLOUT));
}

UTEST(vmnl_net_socket_system_poll_events, converts_no_event)
{
    EXPECT_EQ(vmnl_net_socket_system_poll_events(0), (short) 0);
}
