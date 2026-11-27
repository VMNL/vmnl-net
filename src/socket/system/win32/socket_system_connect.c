#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

#include <mswsock.h>

bool vmnl_net_socket_system_connect(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen;
    int wsa_error;

    if (vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &socklen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return false;
    }
    if (vmnl_net_socket_system_check_remote(netsock, address, error) == false) {
        return false;
    }
    if (connect(netsock->handle, (const struct sockaddr *) &storage, socklen) == SOCKET_ERROR) {
        wsa_error = WSAGetLastError();
        vmnl_net_error_set(error, (wsa_error == WSAEWOULDBLOCK) ? VMNL_NET_EINPROGRESS : vmnl_net_error_from_system(wsa_error));
        return false;
    }
    if (netsock->type == SOCK_DGRAM && vmnl_net_socket_system_report_unreachable(netsock, SIO_UDP_CONNRESET, true) == false) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    netsock->flags |= VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
