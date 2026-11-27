#define _GNU_SOURCE

#include "../socket_datagram_tests.h"

#include <limits.h>

static VmnlNetBufferView views[IOV_MAX + 1];

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_send, types)

UTEST_I(vmnl_net_socket_datagram_send, sends_iov_max_views, NTYPES)
{
    uint8_t byte = 'x';

    views_on_byte(views, IOV_MAX, &byte);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, views, IOV_MAX, NULL), (size_t) IOV_MAX);
}

UTEST_I(vmnl_net_socket_datagram_send, fails_with_more_views_than_iov_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 'x';

    views_on_byte(views, IOV_MAX + 1, &byte);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, views, IOV_MAX + 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

#ifdef __APPLE__
UTEST_I(vmnl_net_socket_datagram_send, rejects_views_above_int_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 'x';
    VmnlNetBufferView large[2] = {{.data = &byte, .size = (size_t) INT_MAX / 2 + 1}, {.data = &byte, .size = (size_t) INT_MAX / 2 + 1}};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, large, 2, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
#endif
