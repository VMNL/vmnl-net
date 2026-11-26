#include "address/address_internal.h"
#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_local_address(const struct VmnlNetSocket *netsock, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = sizeof storage;

    if (getsockname(netsock->handle, (struct sockaddr *) &storage, &socklen) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    (void) vmnl_net_address_from_sockaddr(&storage, address);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
