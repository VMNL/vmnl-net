#include "socket/socket_internal.h"

bool vmnl_net_socket_shutdown(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    if (shutdown(netsock->handle, VMNL_NET_SOCKET_SHUTDOWN_SEND) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
