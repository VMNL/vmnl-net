#define _GNU_SOURCE

#include "../socket_datagram_tests.h"

#include <limits.h>

static VmnlNetBufferView views[IOV_MAX + 1];

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_receive, types)

UTEST_I(vmnl_net_socket_datagram_receive, receives_into_iov_max_views, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 0;

    views_on_byte(views, IOV_MAX, &byte);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_waiting(utest_fixture->receiver, views, IOV_MAX, &error), (size_t) 5);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST_I(vmnl_net_socket_datagram_receive, fails_with_more_views_than_iov_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 0;

    views_on_byte(views, IOV_MAX + 1, &byte);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_waiting(utest_fixture->receiver, views, IOV_MAX + 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

#ifdef __APPLE__
UTEST_I(vmnl_net_socket_datagram_receive, rejects_views_above_int_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 0;
    VmnlNetBufferView large[2] = {{.data = &byte, .size = (size_t) INT_MAX / 2 + 1}, {.data = &byte, .size = (size_t) INT_MAX / 2 + 1}};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->receiver, large, 2, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
#endif
