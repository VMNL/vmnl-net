#include "socket/system/socket_system_internal.h"

#include <fcntl.h>

bool vmnl_net_socket_system_non_blocking(struct VmnlNetSocketSystem *netsock)
{
    return fcntl(netsock->handle, F_SETFL, O_NONBLOCK) != -1;
}
