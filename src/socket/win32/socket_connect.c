#include "address/address_internal.h"
#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_connect(struct VmnlNetSocket *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen;

    if (vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &socklen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return false;
    }
    if (connect(netsock->handle, (const struct sockaddr *) &storage, socklen) == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            vmnl_net_error_set(error, VMNL_NET_EINPROGRESS);
            return false;
        }
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    if (netsock->type == SOCK_DGRAM && vmnl_net_socket_report_unreachable(netsock, SIO_UDP_CONNRESET, true) == false) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
