#include "socket/socket_internal.h"

#include <fcntl.h>

bool vmnl_net_socket_non_blocking(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    int status = fcntl(netsock->handle, F_GETFL);

    if (status == -1 || fcntl(netsock->handle, F_SETFL, (unsigned int) status | O_NONBLOCK) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
