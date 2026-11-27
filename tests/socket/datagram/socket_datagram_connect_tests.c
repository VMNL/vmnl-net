#include "socket_datagram_tests.h"

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_connect, types)

UTEST_I(vmnl_net_socket_datagram_connect, sets_peer_of_send, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t bytes[2] = {'h', 'i'};

    EXPECT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    ASSERT_EQ(vmnl_net_socket_datagram_send(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, NULL), (size_t) 2);
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "hi"));
}

UTEST_I(vmnl_net_socket_datagram_connect, drops_datagrams_from_other_senders, NTYPES)
{
    VmnlNetSocketDatagram *other = create(utest_fixture->type);

    ASSERT_NE(other, NULL);
    EXPECT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->receiver, &utest_fixture->sender_address, NULL));
    ASSERT_TRUE(send_to(other, &utest_fixture->receiver_address, "no"));
    ASSERT_TRUE(send_to(utest_fixture->sender, &utest_fixture->receiver_address, "hi"));
    EXPECT_TRUE(datagram_equals(utest_fixture->receiver, "hi"));
    vmnl_net_socket_datagram_destroy(other);
}

UTEST_I(vmnl_net_socket_datagram_connect, fails_with_broadcast_address, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("255.255.255.255", 9, &address, NULL));
    EXPECT_FALSE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EACCES);
}

UTEST(vmnl_net_socket_datagram_connect, fails_with_ipv6_address_on_ipv4_socket)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetSocketDatagram *netsock = create(&types[0]);
    VmnlNetAddress address;

    ASSERT_NE(context, NULL);
    ASSERT_NE(netsock, NULL);
    ASSERT_TRUE(vmnl_net_address_make("::1", 9, &address, NULL));
    EXPECT_FALSE(vmnl_net_socket_datagram_connect(netsock, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EAFNOSUPPORT);
    vmnl_net_socket_datagram_destroy(netsock);
    vmnl_net_context_destroy(context);
}

UTEST_I(vmnl_net_socket_datagram_connect, keeps_refusal_for_next_receive, NTYPES)
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
    EXPECT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_socket_datagram_receive(utest_fixture->sender, &(VmnlNetBufferView) {.data = bytes, .size = 2}, 1, &error), (size_t) 0);
    EXPECT_EQ(error, VMNL_NET_ECONNREFUSED);
}

UTEST_I(vmnl_net_socket_datagram_connect, accepts_null_error, NTYPES)
{
    EXPECT_TRUE(vmnl_net_socket_datagram_connect(utest_fixture->sender, &utest_fixture->receiver_address, NULL));
}
