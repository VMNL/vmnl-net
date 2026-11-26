#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

bool vmnl_net_socket_set_send_buffer_size(struct VmnlNetSocket *netsock, size_t size, VmnlNetError *error)
{
    int option_value;

    if (size > (size_t) INT_MAX) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    option_value = (int) size;
    if (setsockopt(netsock->handle, SOL_SOCKET, SO_SNDBUF, (const char *) &option_value, sizeof option_value) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
