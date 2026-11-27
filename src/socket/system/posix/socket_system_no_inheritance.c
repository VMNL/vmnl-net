#include "socket/system/socket_system_internal.h"

#include <fcntl.h>

bool vmnl_net_socket_system_no_inheritance(struct VmnlNetSocketSystem *netsock, VmnlNetError *error)
{
    /* GCOVR_EXCL_START: fcntl() only fails on a descriptor that is not open. */
    if (fcntl(netsock->handle, F_SETFD, FD_CLOEXEC) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    /* GCOVR_EXCL_STOP */
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
