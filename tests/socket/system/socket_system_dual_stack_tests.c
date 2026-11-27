#include "../datagram/socket_datagram_tests.h"
#include "socket/system/socket_system_internal.h"

UTEST(vmnl_net_socket_system_dual_stack, clears_ipv6_only)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetSocketDatagram *netsock = create(&types[1]);
    struct VmnlNetSocketSystem system = {.handle = vmnl_net_socket_datagram_handle(netsock), .family = AF_INET6, .type = SOCK_DGRAM};
    int ipv6_only = 1;
    socklen_t length = sizeof ipv6_only;

    ASSERT_NE(context, NULL);
    ASSERT_NE(netsock, NULL);
    ASSERT_EQ(setsockopt(system.handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char *) &ipv6_only, sizeof ipv6_only), 0);
    EXPECT_TRUE(vmnl_net_socket_system_dual_stack(&system, NULL));
    ASSERT_EQ(getsockopt(system.handle, IPPROTO_IPV6, IPV6_V6ONLY, (char *) &ipv6_only, &length), 0);
    EXPECT_EQ(ipv6_only, 0);
    vmnl_net_socket_datagram_destroy(netsock);
    vmnl_net_context_destroy(context);
}
