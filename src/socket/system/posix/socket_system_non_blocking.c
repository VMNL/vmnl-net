#include "socket/system/socket_system_internal.h"

#include <fcntl.h>

bool vmnl_net_socket_system_non_blocking(struct VmnlNetSocketSystem *netsock, VmnlNetError *error)
{
    int status = fcntl(netsock->handle, F_GETFL);

    /* GCOVR_EXCL_START: fcntl() only fails on a descriptor that is not open. */
    if (status == -1 || fcntl(netsock->handle, F_SETFL, (unsigned int) status | O_NONBLOCK) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    /* GCOVR_EXCL_STOP */
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
