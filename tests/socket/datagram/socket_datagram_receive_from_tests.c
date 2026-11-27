#include "socket_datagram_tests.h"

#include <limits.h>

static VmnlNetBufferView max_views[VMNL_NET_SOCKET_MAX_VIEWS + 1];

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_receive_from, types)

UTEST_I(vmnl_net_socket_datagram_receive_from, receives_datagram_and_sender, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};
    VmnlNetAddress address;

    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_from_waiting(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, &error), (size_t) 5);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(memcmp(received, "hello", 5), 0);
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &utest_fixture->sender_address));
}

UTEST_I(vmnl_net_socket_datagram_receive_from, receives_empty_datagram, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};
    VmnlNetAddress address;

    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, ""));
    EXPECT_EQ(receive_from_waiting(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, scatters_datagram_over_views, NTYPES)
{
    uint8_t received[5] = {0};
    VmnlNetBufferView views[2] = {{.data = received, .size = 3}, {.data = received + 3, .size = 2}};
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_from_waiting(utest_fixture->receiver, views, 2, &address, &error), (size_t) 5);
    EXPECT_EQ(memcmp(received, "hello", 5), 0);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, fails_when_empty, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};
    VmnlNetAddress address;

    EXPECT_EQ(vmnl_net_socket_datagram_receive_from(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EAGAIN);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, fails_when_empty_after_datagram_to_closed_port, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetSocketDatagram *closed = create(utest_fixture->type);
    VmnlNetAddress address;
    uint8_t received[8] = {0};

    ASSERT_NE(closed, NULL);
    ASSERT_TRUE(bind_loopback(closed, utest_fixture->type, &address));
    vmnl_net_socket_datagram_destroy(closed);
    ASSERT_TRUE(send_to(utest_fixture->sender, &address, "hi"));
    EXPECT_EQ(receive_from_waiting(utest_fixture->sender, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EAGAIN);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, fails_after_datagram_to_closed_port_when_connected, NTYPES)
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
    EXPECT_EQ(receive_from_waiting(utest_fixture->sender, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_ECONNREFUSED);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, keeps_address_on_failure, NTYPES)
{
    uint8_t received[8] = {0};
    VmnlNetAddress address = utest_fixture->receiver_address;

    EXPECT_EQ(vmnl_net_socket_datagram_receive_from(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, NULL), (size_t) 0);
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &utest_fixture->receiver_address));
}

UTEST_I(vmnl_net_socket_datagram_receive_from, fails_with_views_smaller_than_datagram, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[2] = {0};
    VmnlNetAddress address;

    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_from_waiting(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 2}, 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, rejects_zero_views, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t received[8] = {0};
    VmnlNetAddress address;

    EXPECT_EQ(vmnl_net_socket_datagram_receive_from(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 0, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, accepts_null_error, NTYPES)
{
    uint8_t received[8] = {0};
    VmnlNetAddress address;

    EXPECT_EQ(vmnl_net_socket_datagram_receive_from(utest_fixture->receiver, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, NULL), (size_t) 0);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, receives_into_max_views, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;
    uint8_t byte = 0;

    views_on_byte(max_views, VMNL_NET_SOCKET_MAX_VIEWS, &byte);
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_from_waiting(utest_fixture->receiver, max_views, VMNL_NET_SOCKET_MAX_VIEWS, &address, &error), (size_t) 5);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, fails_with_more_views_than_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;
    uint8_t byte = 0;

    views_on_byte(max_views, VMNL_NET_SOCKET_MAX_VIEWS + 1, &byte);
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hello"));
    EXPECT_EQ(receive_from_waiting(utest_fixture->receiver, max_views, VMNL_NET_SOCKET_MAX_VIEWS + 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, rejects_views_above_int_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;
    uint8_t byte = 0;
    VmnlNetBufferView huge[2] = {{.data = &byte, .size = (size_t) INT_MAX / 2 + 1}, {.data = &byte, .size = (size_t) INT_MAX / 2 + 1}};

    EXPECT_EQ(vmnl_net_socket_datagram_receive_from(utest_fixture->receiver, huge, 2, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_receive_from, fails_when_unbound, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetSocketDatagram *unbound = create(utest_fixture->type);
    VmnlNetAddress address;
    uint8_t received[8] = {0};

    ASSERT_NE(unbound, NULL);
    EXPECT_EQ(vmnl_net_socket_datagram_receive_from(unbound, &(VmnlNetBufferView) {.data = received, .size = 8}, 1, &address, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EAGAIN);
    vmnl_net_socket_datagram_destroy(unbound);
}
