#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_set_broadcast(struct VmnlNetSocketSystem *netsock, bool enabled, VmnlNetError *error)
{
    /* GCOVR_EXCL_START: SO_BROADCAST only fails on a socket that is not open. */
    if (vmnl_net_socket_system_set_option(netsock, SOL_SOCKET, SO_BROADCAST, (int) enabled, error) == false) {
        return false;
    }
    /* GCOVR_EXCL_STOP */
    if (enabled == true) {
        netsock->flags |= VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST;
    } else {
        netsock->flags &= ~VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST;
    }
    return true;
}
