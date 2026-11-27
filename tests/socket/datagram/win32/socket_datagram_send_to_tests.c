#include "../socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_send_to, types)

UTEST_I(vmnl_net_socket_datagram_send_to, fails_with_wildcard_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_TRUE(vmnl_net_address_make(utest_fixture->type->wildcard, 9, &address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->netsock, &address, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EADDRNOTAVAIL);
}
