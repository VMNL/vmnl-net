#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_no_inheritance(struct VmnlNetSocketSystem *netsock)
{
    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    return SetHandleInformation((HANDLE) netsock->handle, HANDLE_FLAG_INHERIT, 0) != false;
}
