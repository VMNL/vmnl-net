#include "socket_datagram_tests.h"

static size_t deallocations;

static void counting_deallocate(void *pointer)
{
    deallocations++;
    free(pointer);
}

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_destroy, types)

UTEST_I(vmnl_net_socket_datagram_destroy, frees_socket, NTYPES)
{
    vmnl_net_allocator_set(malloc, realloc, counting_deallocate);
    deallocations = 0;
    vmnl_net_socket_datagram_destroy(utest_fixture->netsock);
    utest_fixture->netsock = NULL;
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(deallocations, (size_t) 1);
}

UTEST_I(vmnl_net_socket_datagram_destroy, closes_system_socket, NTYPES)
{
    VmnlNetSocketDatagram *other = create(utest_fixture->type);
    VmnlNetAddress address;

    ASSERT_NE(other, NULL);
    ASSERT_TRUE(bind_loopback(utest_fixture->netsock, utest_fixture->type, &address));
    vmnl_net_socket_datagram_destroy(utest_fixture->netsock);
    utest_fixture->netsock = NULL;
    EXPECT_TRUE(vmnl_net_socket_datagram_bind(other, &address, NULL));
    vmnl_net_socket_datagram_destroy(other);
}

UTEST(vmnl_net_socket_datagram_destroy, ignores_null_socket)
{
    vmnl_net_allocator_set(malloc, realloc, counting_deallocate);
    deallocations = 0;
    vmnl_net_socket_datagram_destroy(NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(deallocations, (size_t) 0);
}
