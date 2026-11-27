#include "socket_datagram_tests.h"

static uint8_t large[65536];

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_send_to, types)

UTEST_I(vmnl_net_socket_datagram_send_to, sends_datagram_to_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[5] = {'h', 'e', 'l', 'l', 'o'};

    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = bytes, .size = 5}, 1, &error), (size_t) 5);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "hello"));
}

UTEST_I(vmnl_net_socket_datagram_send_to, sends_empty_datagram, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[1] = {0};

    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = bytes, .size = 0}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, ""));
}

UTEST_I(vmnl_net_socket_datagram_send_to, sends_views_as_one_datagram, NTYPES)
{
    uint8_t bytes[5] = {'h', 'e', 'l', 'l', 'o'};
    VmnlNetBufferView views[2] = {{.data = bytes, .size = 3}, {.data = bytes + 3, .size = 2}};

    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, views, 2, NULL), (size_t) 5);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "hello"));
}

UTEST_I(vmnl_net_socket_datagram_send_to, keeps_boundaries_between_datagrams, NTYPES)
{
    uint8_t bytes[5] = {'h', 'e', 'l', 'l', 'o'};

    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = bytes, .size = 3}, 1, NULL), (size_t) 3);
    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = bytes + 3, .size = 2}, 1, NULL), (size_t) 2);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "hel"));
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "lo"));
}

UTEST(vmnl_net_socket_datagram_send_to, reaches_ipv4_peer_from_ipv6_socket)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetSocketDatagram *sender = create(&types[1]);
    VmnlNetSocketDatagram *receiver = create(&types[0]);
    VmnlNetAddress receiver_address;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_NE(context, NULL);
    ASSERT_NE(sender, NULL);
    ASSERT_NE(receiver, NULL);
    ASSERT_TRUE(bind_loopback(receiver, &types[0], &receiver_address));
    EXPECT_EQ(vmnl_net_socket_datagram_send_to(sender, &receiver_address, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 2);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(datagram_equals(receiver, "hi"));
    vmnl_net_socket_datagram_destroy(sender);
    vmnl_net_socket_datagram_destroy(receiver);
    vmnl_net_context_destroy(context);
}

UTEST_I(vmnl_net_socket_datagram_send_to, fails_with_datagram_above_64_kib, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = large, .size = 65536}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

UTEST_I(vmnl_net_socket_datagram_send_to, fails_with_broadcast_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_TRUE(vmnl_net_address_make("255.255.255.255", 9, &address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &address, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EACCES);
}

UTEST(vmnl_net_socket_datagram_send_to, fails_with_ipv6_address_on_ipv4_socket)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetSocketDatagram *netsock = create(&types[0]);
    VmnlNetAddress address;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_NE(context, NULL);
    ASSERT_NE(netsock, NULL);
    ASSERT_TRUE(vmnl_net_address_make("::1", 9, &address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send_to(netsock, &address, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EAFNOSUPPORT);
    vmnl_net_socket_datagram_destroy(netsock);
    vmnl_net_context_destroy(context);
}

UTEST_I(vmnl_net_socket_datagram_send_to, fails_when_connected, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[2] = {'h', 'i'};

    ASSERT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EISCONN);
}

UTEST_I(vmnl_net_socket_datagram_send_to, rejects_zero_views, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[2] = {'h', 'i'};

    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 0, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_send_to, accepts_null_error, NTYPES)
{
    uint8_t bytes[2] = {'h', 'i'};

    EXPECT_EQ(vmnl_net_socket_datagram_send_to(utest_fixture->sender, &utest_fixture->receiver_address, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, NULL), (size_t) 2);
}
