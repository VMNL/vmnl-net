#include "socket/socket_internal.h"

bool vmnl_net_socket_open(int family, int type, struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    struct VmnlNetSocket sock = {.handle = socket(family, type, 0), .family = family, .type = type};

    if (sock.handle == VMNL_NET_SOCKET_INVALID) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    if (vmnl_net_socket_non_blocking(&sock, error) == false || vmnl_net_socket_no_inheritance(&sock, error) == false || (family == AF_INET6 && vmnl_net_socket_dual_stack(&sock, error) == false)) {
        vmnl_net_socket_close(&sock);
        return false;
    }
    *netsock = sock;
    return true;
}
