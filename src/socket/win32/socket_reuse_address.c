#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_reuse_address(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    BOOL option_value = TRUE;

    if (setsockopt(netsock->handle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char *) &option_value, (int) sizeof option_value) == SOCKET_ERROR) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
