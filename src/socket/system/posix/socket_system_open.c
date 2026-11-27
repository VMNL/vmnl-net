#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_open(int family, int type, struct VmnlNetSocketSystem *netsock, VmnlNetError *error)
{
    struct VmnlNetSocketSystem sock = {.handle = socket(family, type, 0), .family = family, .type = type};

    if (sock.handle == VMNL_NET_SOCKET_SYSTEM_INVALID) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    /* GCOVR_EXCL_START: the settings of a socket just opened only fail if it is not open. */
    if (vmnl_net_socket_system_non_blocking(&sock, error) == false || vmnl_net_socket_system_no_inheritance(&sock, error) == false || (family == AF_INET6 && vmnl_net_socket_system_dual_stack(&sock, error) == false)) {
        vmnl_net_socket_system_close(&sock);
        return false;
    }
    /* GCOVR_EXCL_STOP */
    *netsock = sock;
    return true;
}
