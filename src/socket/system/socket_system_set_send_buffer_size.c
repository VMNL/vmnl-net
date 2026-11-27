#include "socket/system/socket_system_internal.h"

#include <limits.h>

bool vmnl_net_socket_system_set_send_buffer_size(struct VmnlNetSocketSystem *netsock, size_t size, VmnlNetError *error)
{
    if (size > (size_t) INT_MAX) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    return vmnl_net_socket_system_set_option(netsock, SOL_SOCKET, SO_SNDBUF, (int) size, error);
}
