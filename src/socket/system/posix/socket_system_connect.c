#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

#include <errno.h>

bool vmnl_net_socket_system_connect(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen;

    if (vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &socklen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return false;
    }
    if (vmnl_net_socket_system_check_remote(netsock, address, error) == false) {
        return false;
    }
    if (connect(netsock->handle, (const struct sockaddr *) &storage, socklen) == -1) {
        if (errno != ECONNREFUSED || netsock->type != SOCK_DGRAM) { /* GCOVR_EXCL_BR_LINE: only macOS reports a pending refusal at the connect, which still connects, and the coverage runs on Linux. */
            vmnl_net_error_set(error, vmnl_net_error_from_last_system());
            return false;
        }
        netsock->flags |= VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED; /* GCOVR_EXCL_LINE: see the refusal above. */
    }
    netsock->flags |= VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
