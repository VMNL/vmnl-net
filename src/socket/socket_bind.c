#include "address/address_internal.h"
#include "socket/socket_internal.h"

bool vmnl_net_socket_bind(struct VmnlNetSocket *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen;

    if (vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &socklen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return false;
    }
    if (bind(netsock->handle, (const struct sockaddr *) &storage, socklen) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
