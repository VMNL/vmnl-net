#include "socket/system/socket_system_internal.h"

#include <unistd.h>

void vmnl_net_socket_system_close(struct VmnlNetSocketSystem *netsock)
{
    close(netsock->handle);
}
