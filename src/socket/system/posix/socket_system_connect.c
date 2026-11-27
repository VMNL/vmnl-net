#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

#include <errno.h>

bool vmnl_net_socket_system_connect(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen;
    int result;

    if (vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &socklen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return false;
    }
    if (vmnl_net_socket_system_check_remote(netsock, address, error) == false) {
        return false;
    }
    result = connect(netsock->handle, (const struct sockaddr *) &storage, socklen);
    /* GCOVR_EXCL_START: only macOS reports a pending refusal at the connect, and the coverage runs on Linux. */
    if (result == -1 && errno == ECONNREFUSED && netsock->type == SOCK_DGRAM) {
        netsock->flags |= VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED;
        result = connect(netsock->handle, (const struct sockaddr *) &storage, socklen);
    }
    /* GCOVR_EXCL_STOP */
    if (result == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    netsock->flags |= VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
