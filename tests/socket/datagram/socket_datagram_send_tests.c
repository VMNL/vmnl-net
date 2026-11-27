#include "socket_datagram_tests.h"

#include <limits.h>

static uint8_t large[65536];

static VmnlNetBufferView max_views[VMNL_NET_SOCKET_MAX_VIEWS + 1];

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_send, types)

UTEST_I(vmnl_net_socket_datagram_send, sends_datagram_to_peer, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[5] = {'h', 'e', 'l', 'l', 'o'};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 5}, 1, &error), (size_t) 5);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "hello"));
}

UTEST_I(vmnl_net_socket_datagram_send, sends_views_as_one_datagram, NTYPES)
{
    uint8_t bytes[5] = {'h', 'e', 'l', 'l', 'o'};
    VmnlNetBufferView views[2] = {{.data = bytes, .size = 3}, {.data = bytes + 3, .size = 2}};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, views, 2, NULL), (size_t) 5);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "hello"));
}

UTEST_I(vmnl_net_socket_datagram_send, sends_empty_datagram, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[1] = {0};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 0}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, ""));
}

UTEST_I(vmnl_net_socket_datagram_send, fails_without_peer, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[2] = {'h', 'i'};

    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_ENOTCONN);
}

UTEST_I(vmnl_net_socket_datagram_send, fails_with_datagram_above_64_kib, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = large, .size = 65536}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

UTEST_I(vmnl_net_socket_datagram_send, keeps_refusal_for_next_receive, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetSocketDatagram *closed = create(utest_fixture->type);
    VmnlNetAddress address;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_NE(closed, NULL);
    ASSERT_TRUE(bind_loopback(closed, utest_fixture->type, &address));
    vmnl_net_socket_datagram_destroy(closed);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &address, NULL));
    ASSERT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, NULL), (size_t) 2);
    ASSERT_EQ(vmnl_net_socket_datagram_wait_for(utest_fixture->sender, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, NULL), VMNL_NET_SOCKET_READABLE);
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 2);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_ECONNREFUSED);
}

UTEST_I(vmnl_net_socket_datagram_send, rejects_zero_views, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 0, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_send, accepts_null_error, NTYPES)
{
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, NULL), (size_t) 2);
}

UTEST_I(vmnl_net_socket_datagram_send, sends_max_views, NTYPES)
{
    uint8_t byte = 'x';

    views_on_byte(max_views, VMNL_NET_SOCKET_MAX_VIEWS, &byte);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, max_views, VMNL_NET_SOCKET_MAX_VIEWS, NULL), (size_t) VMNL_NET_SOCKET_MAX_VIEWS);
}

UTEST_I(vmnl_net_socket_datagram_send, fails_with_more_views_than_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 'x';

    views_on_byte(max_views, VMNL_NET_SOCKET_MAX_VIEWS + 1, &byte);
    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, max_views, VMNL_NET_SOCKET_MAX_VIEWS + 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

UTEST_I(vmnl_net_socket_datagram_send, rejects_views_above_int_max, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 'x';
    VmnlNetBufferView huge[2] = {{.data = &byte, .size = (size_t) INT_MAX / 2 + 1}, {.data = &byte, .size = (size_t) INT_MAX / 2 + 1}};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, huge, 2, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
