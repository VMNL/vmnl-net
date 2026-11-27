#include "socket/system/socket_system_internal.h"

#include "utest.h"

UTEST(vmnl_net_socket_system_ready_events, returns_readable_on_pollin)
{
    EXPECT_EQ(vmnl_net_socket_system_ready_events(VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE, POLLIN), VMNL_NET_SOCKET_READABLE);
}

UTEST(vmnl_net_socket_system_ready_events, returns_writable_on_pollout)
{
    EXPECT_EQ(vmnl_net_socket_system_ready_events(VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE, POLLOUT), VMNL_NET_SOCKET_WRITABLE);
}

UTEST(vmnl_net_socket_system_ready_events, keeps_events_asked_only)
{
    EXPECT_EQ(vmnl_net_socket_system_ready_events(VMNL_NET_SOCKET_READABLE, (short) ((unsigned int) POLLIN | (unsigned int) POLLOUT)), VMNL_NET_SOCKET_READABLE);
}

UTEST(vmnl_net_socket_system_ready_events, returns_events_asked_on_pollerr)
{
    EXPECT_EQ(vmnl_net_socket_system_ready_events(VMNL_NET_SOCKET_READABLE, POLLERR), VMNL_NET_SOCKET_READABLE);
}
