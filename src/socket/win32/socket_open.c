#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>
#include <stddef.h>

bool vmnl_net_socket_open(int family, int type, struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    struct VmnlNetSocket sock = {.handle = WSASocketW(family, type, 0, NULL, 0, WSA_FLAG_OVERLAPPED | WSA_FLAG_NO_HANDLE_INHERIT), .family = family, .type = type};

    if (sock.handle == VMNL_NET_SOCKET_INVALID) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    if (vmnl_net_socket_non_blocking(&sock, error) == false || (family == AF_INET6 && vmnl_net_socket_dual_stack(&sock, error) == false)) {
        vmnl_net_socket_close(&sock);
        return false;
    }
    if (type == SOCK_DGRAM && (vmnl_net_socket_report_unreachable(&sock, SIO_UDP_CONNRESET, false) == false || vmnl_net_socket_report_unreachable(&sock, SIO_UDP_NETRESET, false) == false)) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        vmnl_net_socket_close(&sock);
        return false;
    }
    *netsock = sock;
    return true;
}
