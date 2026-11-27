#define _GNU_SOURCE

#include "../socket_datagram_tests.h"
#include "socket/system/socket_system_internal.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_set_send_buffer_size, types)

#ifdef __APPLE__
UTEST_I(vmnl_net_socket_datagram_set_send_buffer_size, rejects_zero_size, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_socket_datagram_set_send_buffer_size(utest_fixture->netsock, 0, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
#endif

#ifdef __linux__
UTEST_I(vmnl_net_socket_datagram_set_send_buffer_size, doubles_size, NTYPES)
{
    int size = 0;
    socklen_t length = sizeof size;

    ASSERT_TRUE(vmnl_net_socket_datagram_set_send_buffer_size(utest_fixture->netsock, 65536, NULL));
    ASSERT_EQ(getsockopt(vmnl_net_socket_datagram_handle(utest_fixture->netsock), SOL_SOCKET, SO_SNDBUF, (char *) &size, &length), 0);
    EXPECT_EQ(size, 131072);
}
#endif
