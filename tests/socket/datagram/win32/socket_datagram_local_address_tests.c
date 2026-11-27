#include "../socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_local_address, types)

UTEST_I(vmnl_net_socket_datagram_local_address, fails_when_unbound, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_socket_datagram_local_address(utest_fixture->netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_local_address, keeps_address_on_failure, NTYPES)
{
    VmnlNetAddress address;
    VmnlNetAddress kept;

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->loopback, 9, &address, NULL));
    kept = address;
    EXPECT_FALSE(vmnl_net_socket_datagram_local_address(utest_fixture->netsock, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &kept));
}
