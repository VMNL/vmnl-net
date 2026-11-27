#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_listen(struct VmnlNetSocketSystem *netsock, VmnlNetError *error)
{
    if (listen(netsock->handle, SOMAXCONN) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
