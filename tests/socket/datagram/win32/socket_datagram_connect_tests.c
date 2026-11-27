#include "../socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_connect, types)

UTEST_I(vmnl_net_socket_datagram_connect, fails_with_wildcard_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->wildcard, 9, &address, NULL));
    EXPECT_FALSE(vmnl_net_socket_datagram_connect(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EADDRNOTAVAIL);
}
