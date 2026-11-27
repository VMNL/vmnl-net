#include "socket_datagram_tests.h"

struct vmnl_net_socket_datagram_create {
    const struct DatagramType *type;
    VmnlNetContext *context;
    VmnlNetSocketDatagram *netsock;
};

static void *failing_allocate(size_t size)
{
    (void) size;
    return NULL;
}

UTEST_I_SETUP(vmnl_net_socket_datagram_create)
{
    utest_fixture->type = &types[utest_index];
    utest_fixture->context = vmnl_net_context_create(NULL);
    ASSERT_NE(utest_fixture->context, NULL);
}

UTEST_I_TEARDOWN(vmnl_net_socket_datagram_create)
{
    (void) utest_result;
    (void) utest_index;
    vmnl_net_allocator_set(malloc, realloc, free);
    vmnl_net_socket_datagram_destroy(utest_fixture->netsock);
    vmnl_net_context_destroy(utest_fixture->context);
}

UTEST_I(vmnl_net_socket_datagram_create, creates_socket, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    utest_fixture->netsock = vmnl_net_socket_datagram_create(utest_fixture->type->type, utest_fixture->type->version, &error);
    EXPECT_NE(utest_fixture->netsock, NULL);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST(vmnl_net_socket_datagram_create, rejects_unknown_type)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_socket_datagram_create(0, VMNL_NET_ADDRESS_IPV4, &error), NULL);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    error = UINT32_MAX;
    EXPECT_EQ(vmnl_net_socket_datagram_create(VMNL_NET_SOCKET_DATAGRAM_UDP + 1, VMNL_NET_ADDRESS_IPV4, &error), NULL);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_socket_datagram_create, rejects_unknown_version)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_socket_datagram_create(VMNL_NET_SOCKET_DATAGRAM_UDP, VMNL_NET_ADDRESS_IPV4 + 1, &error), NULL);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    error = UINT32_MAX;
    EXPECT_EQ(vmnl_net_socket_datagram_create(VMNL_NET_SOCKET_DATAGRAM_UDP, VMNL_NET_ADDRESS_IPV6 + 1, &error), NULL);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_socket_datagram_create, fails_without_memory, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    vmnl_net_allocator_set(failing_allocate, realloc, free);
    utest_fixture->netsock = vmnl_net_socket_datagram_create(utest_fixture->type->type, utest_fixture->type->version, &error);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(utest_fixture->netsock, NULL);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
}

UTEST_I(vmnl_net_socket_datagram_create, accepts_null_error, NTYPES)
{
    utest_fixture->netsock = vmnl_net_socket_datagram_create(utest_fixture->type->type, utest_fixture->type->version, NULL);
    EXPECT_NE(utest_fixture->netsock, NULL);
}
