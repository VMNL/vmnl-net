#include "../socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_receive_from, types)

UTEST_I(vmnl_net_socket_datagram_receive_from, fails_when_unbound, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;
    uint8_t received[8] = {0};

    EXPECT_EQ(vmnl_net_socket_datagram_receive_from(utest_fixture->netsock, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
