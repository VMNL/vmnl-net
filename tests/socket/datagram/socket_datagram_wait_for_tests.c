#include "socket_datagram_tests.h"

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_wait_for, types)

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_readable_when_datagram_waits, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hi"));
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, &error), VMNL_NET_SOCKET_READABLE);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_writable_with_room_to_send, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, VMNL_NET_SOCKET_WRITABLE, 0, &error), VMNL_NET_SOCKET_WRITABLE);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_both_events_when_both_came, NTYPES)
{
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hi"));
    ASSERT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, NULL), VMNL_NET_SOCKET_READABLE);
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE, 0, NULL), VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_only_events_that_came, NTYPES)
{
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE, 0, NULL), VMNL_NET_SOCKET_WRITABLE);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_readable_when_error_waits, NTYPES)
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
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, NULL), VMNL_NET_SOCKET_READABLE);
    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->sender, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_ECONNREFUSED);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_readable_when_refusal_waits_after_send, NTYPES)
{
    VmnlNetSocketDatagram *closed = create(utest_fixture->type);
    VmnlNetAddress address;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_NE(closed, NULL);
    ASSERT_TRUE(bind_loopback(closed, utest_fixture->type, &address));
    vmnl_net_socket_datagram_destroy(closed);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &address, NULL));
    ASSERT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, NULL), (size_t) 2);
    ASSERT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, NULL), VMNL_NET_SOCKET_READABLE);
    ASSERT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, NULL), (size_t) 2);
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, VMNL_NET_SOCKET_READABLE, 0, NULL), VMNL_NET_SOCKET_READABLE);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_readable_with_unlimited_timeout, NTYPES)
{
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hi"));
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, VMNL_NET_CLOCK_INF, NULL), VMNL_NET_SOCKET_READABLE);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, returns_readable_with_timeout_above_int_max_milliseconds, NTYPES)
{
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hi"));
    ASSERT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, NULL), VMNL_NET_SOCKET_READABLE);
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, ((uint64_t) INT_MAX + 1) * MILLISECOND, NULL), VMNL_NET_SOCKET_READABLE);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, fails_after_whole_timeout, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint64_t start = vmnl_net_clock_now();

    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, 20 * MILLISECOND, &error), (VmnlNetSocketEvents) 0);
    EXPECT_EQ(error, VMNL_NET_ETIMEDOUT);
    EXPECT_GE(vmnl_net_clock_elapsed(start), 20 * MILLISECOND);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, fails_at_once_with_zero_timeout, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, 0, &error), (VmnlNetSocketEvents) 0);
    EXPECT_EQ(error, VMNL_NET_ETIMEDOUT);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, rejects_zero_events, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, 0, 0, &error), (VmnlNetSocketEvents) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, rejects_unknown_event, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, VMNL_NET_SOCKET_WRITABLE << 1, 0, &error), (VmnlNetSocketEvents) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_wait_for, accepts_null_error, NTYPES)
{
    EXPECT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, VMNL_NET_SOCKET_WRITABLE, 0, NULL), VMNL_NET_SOCKET_WRITABLE);
}
