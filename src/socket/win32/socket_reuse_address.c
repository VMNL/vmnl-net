#include "socket/socket_internal.h"

bool vmnl_net_socket_reuse_address(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    return vmnl_net_socket_set_option(netsock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, 1, error);
}
