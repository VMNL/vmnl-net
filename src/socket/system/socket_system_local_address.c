#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_local_address(const struct VmnlNetSocketSystem *netsock, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = sizeof storage;

    /* GCOVR_EXCL_START: on POSIX, where the coverage runs, getsockname() only fails on a socket that is not open. */
    if (getsockname(netsock->handle, (struct sockaddr *) &storage, &socklen) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    /* GCOVR_EXCL_STOP */
    (void) vmnl_net_address_from_sockaddr(&storage, address);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
