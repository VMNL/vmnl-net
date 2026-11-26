#include "socket/socket_internal.h"

bool vmnl_net_socket_non_blocking(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    u_long option_value = 1;

    if (ioctlsocket(netsock->handle, FIONBIO, &option_value) == SOCKET_ERROR) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
