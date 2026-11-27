#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_non_blocking(struct VmnlNetSocketSystem *netsock)
{
    u_long option_value = 1;

    return ioctlsocket(netsock->handle, FIONBIO, &option_value) != SOCKET_ERROR;
}
