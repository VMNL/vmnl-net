#include "socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_bind, types)

UTEST_I(vmnl_net_socket_datagram_bind, takes_free_port_with_any_port, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->loopback, VMNL_NET_ADDRESS_ANY_PORT, &address, NULL));
    EXPECT_TRUE(vmnl_net_socket_datagram_bind(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    ASSERT_TRUE(vmnl_net_socket_datagram_local_address(utest_fixture->netsock, &address, NULL));
    EXPECT_NE(address.port, 0);
}

UTEST_I(vmnl_net_socket_datagram_bind, fails_with_address_in_use, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetSocketDatagram *other = create(utest_fixture->type);
    VmnlNetAddress address;

    ASSERT_NE(other, NULL);
    ASSERT_TRUE(bind_loopback(other, utest_fixture->type, &address));
    EXPECT_FALSE(vmnl_net_socket_datagram_bind(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EADDRINUSE);
    vmnl_net_socket_datagram_destroy(other);
}

UTEST_I(vmnl_net_socket_datagram_bind, fails_with_foreign_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->foreign, VMNL_NET_ADDRESS_ANY_PORT, &address, NULL));
    EXPECT_FALSE(vmnl_net_socket_datagram_bind(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EADDRNOTAVAIL);
}

UTEST_I(vmnl_net_socket_datagram_bind, rejects_bound_socket, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(bind_loopback(utest_fixture->netsock, utest_fixture->type, &address));
    EXPECT_FALSE(vmnl_net_socket_datagram_bind(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_socket_datagram_bind, fails_with_ipv6_address_on_ipv4_socket)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetSocketDatagram *netsock = create(&types[0]);
    VmnlNetAddress address;

    ASSERT_NE(context, NULL);
    ASSERT_NE(netsock, NULL);
    ASSERT_TRUE(vmnl_net_address_make("::1", VMNL_NET_ADDRESS_ANY_PORT, &address, NULL));
    EXPECT_FALSE(vmnl_net_socket_datagram_bind(netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EAFNOSUPPORT);
    vmnl_net_socket_datagram_destroy(netsock);
    vmnl_net_context_destroy(context);
}

UTEST_I(vmnl_net_socket_datagram_bind, accepts_null_error, NTYPES)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->loopback, VMNL_NET_ADDRESS_ANY_PORT, &address, NULL));
    EXPECT_TRUE(vmnl_net_socket_datagram_bind(utest_fixture->netsock, &address, NULL));
}
