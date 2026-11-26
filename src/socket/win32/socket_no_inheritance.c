#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_no_inheritance(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    if (SetHandleInformation((HANDLE) netsock->handle, HANDLE_FLAG_INHERIT, 0) == FALSE) {
        vmnl_net_error_set(error, VMNL_NET_FSYSTEM);
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
