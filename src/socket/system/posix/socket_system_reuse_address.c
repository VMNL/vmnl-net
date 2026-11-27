#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_reuse_address(struct VmnlNetSocketSystem *netsock, VmnlNetError *error)
{
    return vmnl_net_socket_system_set_option(netsock, SOL_SOCKET, SO_REUSEADDR, 1, error);
}
