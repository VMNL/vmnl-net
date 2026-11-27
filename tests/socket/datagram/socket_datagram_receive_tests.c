#include "socket_datagram_tests.h"

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_receive, types)

UTEST_I(vmnl_net_socket_datagram_receive, receives_datagram_from_peer, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_waiting(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &error), (size_t) 5);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(memcmp(received, "hello", 5), 0);
}

UTEST_I(vmnl_net_socket_datagram_receive, receives_empty_datagram, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, ""));
    EXPECT_EQ(receive_waiting(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST_I(vmnl_net_socket_datagram_receive, scatters_datagram_over_views, NTYPES)
{
    uint8_t received[5] = {0};
    VmnlNetBufferView views[2] = {{.data = received, .size = 3}, {.data = received + 3, .size = 2}};
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_waiting(utest_fixture->receiver, views, 2, &error), (size_t) 5);
    EXPECT_EQ(memcmp(received, "hello", 5), 0);
}

UTEST_I(vmnl_net_socket_datagram_receive, fails_when_empty, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EAGAIN);
}

UTEST_I(vmnl_net_socket_datagram_receive, fails_with_views_smaller_than_datagram, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[2] = {0};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_waiting(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

UTEST_I(vmnl_net_socket_datagram_receive, fails_after_datagram_to_closed_port, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetSocketDatagram *closed = create(utest_fixture->type);
    VmnlNetAddress address;
    uint8_t received[8] = {0};

    ASSERT_NE(closed, NULL);
    ASSERT_TRUE(bind_loopback(closed, utest_fixture->type, &address));
    vmnl_net_socket_datagram_destroy(closed);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &address, NULL));
    ASSERT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = received, .size = 2}, 1, NULL), (size_t) 2);
    EXPECT_EQ(receive_waiting(utest_fixture->sender, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_ECONNREFUSED);
}

UTEST_I(vmnl_net_socket_datagram_receive, rejects_zero_views, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 0, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_receive, accepts_null_error, NTYPES)
{
    uint8_t received[8] = {0};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, NULL), (size_t) 0);
}
