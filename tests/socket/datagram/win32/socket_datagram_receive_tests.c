#include "../socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_receive, types)

UTEST_I(vmnl_net_socket_datagram_receive, fails_when_unbound, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};

    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->netsock, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
