#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_listen(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    if (listen(netsock->handle, SOMAXCONN) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
