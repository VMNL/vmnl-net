#include "socket/system/socket_system_internal.h"
#include "socket_datagram_tests.h"

DATAGRAM_FIXTURE_I(vmnl_net_socket_datagram_handle, types)

UTEST_I(vmnl_net_socket_datagram_handle, returns_system_socket, NTYPES)
{
    int type = 0;
    socklen_t length = sizeof type;

    ASSERT_EQ(getsockopt(vmnl_net_socket_datagram_handle(utest_fixture->netsock), SOL_SOCKET, SO_TYPE, (char *) &type, &length), 0);
    EXPECT_EQ(type, SOCK_DGRAM);
}
