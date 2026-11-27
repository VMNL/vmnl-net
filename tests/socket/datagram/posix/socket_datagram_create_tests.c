#include "../socket_datagram_tests.h"

#include <sys/resource.h>
#include <unistd.h>

static int lowest_free_descriptor(void)
{
    int descriptor = dup(STDIN_FILENO);

    close(descriptor);
    return descriptor;
}

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_create, types)

UTEST_I(vmnl_net_socket_datagram_create, fails_without_descriptor, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    int descriptor = lowest_free_descriptor();
    VmnlNetSocketDatagram *netsock;
    struct rlimit limit;

    ASSERT_GE(descriptor, 0);
    ASSERT_EQ(getrlimit(RLIMIT_NOFILE, &limit), 0);
    ASSERT_EQ(setrlimit(RLIMIT_NOFILE, &(struct rlimit) {.rlim_cur = (rlim_t) descriptor, .rlim_max = limit.rlim_max}), 0);
    netsock = vmnl_net_socket_datagram_create(utest_fixture->type->type, utest_fixture->type->version, &error);
    setrlimit(RLIMIT_NOFILE, &limit);
    EXPECT_EQ(netsock, NULL);
    EXPECT_EQ(error, VMNL_NET_EMFILE);
    vmnl_net_socket_datagram_destroy(netsock);
}
