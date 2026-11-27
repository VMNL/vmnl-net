#include "socket/system/socket_system_internal.h"

#include <fcntl.h>

bool vmnl_net_socket_system_no_inheritance(struct VmnlNetSocketSystem *netsock)
{
    return fcntl(netsock->handle, F_SETFD, FD_CLOEXEC) != -1;
}
