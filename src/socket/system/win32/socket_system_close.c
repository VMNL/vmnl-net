#include "socket/system/socket_system_internal.h"

void vmnl_net_socket_system_close(struct VmnlNetSocketSystem *netsock)
{
    closesocket(netsock->handle);
}
