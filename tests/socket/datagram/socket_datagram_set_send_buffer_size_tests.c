#include "socket/system/socket_system_internal.h"
#include "socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_set_send_buffer_size, types)

UTEST_I(vmnl_net_socket_datagram_set_send_buffer_size, sets_size_of_64_kib, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    int size = 0;
    socklen_t length = sizeof size;

    EXPECT_TRUE(vmnl_net_socket_datagram_set_send_buffer_size(utest_fixture->netsock, 65536, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    ASSERT_EQ(getsockopt(vmnl_net_socket_datagram_handle(utest_fixture->netsock), SOL_SOCKET, SO_SNDBUF, (char *) &size, &length), 0);
    EXPECT_GE(size, 65536);
}

UTEST_I(vmnl_net_socket_datagram_set_send_buffer_size, rejects_size_above_int_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_socket_datagram_set_send_buffer_size(utest_fixture->netsock, (size_t) INT_MAX + 1, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_socket_datagram_set_send_buffer_size, fails_as_unsupported)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetSocketDatagram netsock = {.ops = &unsupported_ops};

    EXPECT_FALSE(vmnl_net_socket_datagram_set_send_buffer_size(&netsock, 65536, &error));
    EXPECT_EQ(error, VMNL_NET_ENOTSUP);
}

UTEST_I(vmnl_net_socket_datagram_set_send_buffer_size, accepts_null_error, NTYPES)
{
    EXPECT_TRUE(vmnl_net_socket_datagram_set_send_buffer_size(utest_fixture->netsock, 65536, NULL));
}

UTEST_I(vmnl_net_socket_datagram_set_send_buffer_size, rejects_zero_size, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_socket_datagram_set_send_buffer_size(utest_fixture->netsock, 0, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
