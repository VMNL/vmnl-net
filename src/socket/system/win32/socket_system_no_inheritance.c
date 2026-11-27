#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_no_inheritance(struct VmnlNetSocketSystem *netsock, VmnlNetError *error)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    if (SetHandleInformation((HANDLE) netsock->handle, HANDLE_FLAG_INHERIT, 0) == false) {
        vmnl_net_error_set(error, VMNL_NET_FSYSTEM);
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
