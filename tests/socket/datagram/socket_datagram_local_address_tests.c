#include "socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_local_address, types)

UTEST_I(vmnl_net_socket_datagram_local_address, returns_bound_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress bound;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->loopback, VMNL_NET_ADDRESS_ANY_PORT, &bound, NULL));
    ASSERT_TRUE(vmnl_net_socket_datagram_bind(utest_fixture->netsock, &bound, NULL));
    EXPECT_TRUE(vmnl_net_socket_datagram_local_address(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(memcmp(address.ip, bound.ip, sizeof address.ip), 0);
    EXPECT_NE(address.port, 0);
}

UTEST_I(vmnl_net_socket_datagram_local_address, accepts_null_error, NTYPES)
{
    VmnlNetAddress address;

    ASSERT_TRUE(bind_loopback(utest_fixture->netsock, utest_fixture->type, &address));
    EXPECT_TRUE(vmnl_net_socket_datagram_local_address(utest_fixture->netsock, &address, NULL));
}

UTEST_I(vmnl_net_socket_datagram_local_address, gives_wildcard_when_unbound, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;
    VmnlNetAddress wildcard;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->wildcard, VMNL_NET_ADDRESS_ANY_PORT, &wildcard, NULL));
    EXPECT_TRUE(vmnl_net_socket_datagram_local_address(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &wildcard));
}

UTEST_I(vmnl_net_socket_datagram_local_address, gives_port_after_bind_to_wildcard, NTYPES)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->wildcard, VMNL_NET_ADDRESS_ANY_PORT, &address, NULL));
    ASSERT_TRUE(vmnl_net_socket_datagram_bind(utest_fixture->netsock, &address, NULL));
    EXPECT_TRUE(vmnl_net_socket_datagram_local_address(utest_fixture->netsock, &address, NULL));
    EXPECT_NE(address.port, VMNL_NET_ADDRESS_ANY_PORT);
}
