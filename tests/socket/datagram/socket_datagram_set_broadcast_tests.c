#include "socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_set_broadcast, types)

UTEST_I(vmnl_net_socket_datagram_set_broadcast, allows_broadcast_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("255.255.255.255", 9, &address, NULL));
    EXPECT_TRUE(vmnl_net_socket_datagram_set_broadcast(utest_fixture->netsock, true, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->netsock, &address, NULL));
}

UTEST_I(vmnl_net_socket_datagram_set_broadcast, forbids_broadcast_address_after_disable, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("255.255.255.255", 9, &address, NULL));
    ASSERT_TRUE(vmnl_net_socket_datagram_set_broadcast(utest_fixture->netsock, true, NULL));
    EXPECT_TRUE(vmnl_net_socket_datagram_set_broadcast(utest_fixture->netsock, false, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_FALSE(vmnl_net_socket_datagram_connect(utest_fixture->netsock, &address, NULL));
}

UTEST(vmnl_net_socket_datagram_set_broadcast, fails_as_unsupported)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetSocketDatagram netsock = {.ops = &unsupported_ops};

    EXPECT_FALSE(vmnl_net_socket_datagram_set_broadcast(&netsock, true, &error));
    EXPECT_EQ(error, VMNL_NET_ENOTSUP);
}

UTEST_I(vmnl_net_socket_datagram_set_broadcast, accepts_null_error, NTYPES)
{
    EXPECT_TRUE(vmnl_net_socket_datagram_set_broadcast(utest_fixture->netsock, true, NULL));
}
