#include "socket/socket_internal.h"

void vmnl_net_socket_close(struct VmnlNetSocket *netsock)
{
    closesocket(netsock->handle);
}
