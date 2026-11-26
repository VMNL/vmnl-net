#include "socket/socket_internal.h"

#include <unistd.h>

void vmnl_net_socket_close(struct VmnlNetSocket *netsock)
{
    close(netsock->handle);
}
