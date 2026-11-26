#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_set_no_delay(struct VmnlNetSocket *netsock, bool enabled, VmnlNetError *error)
{
    int option_value = (int) enabled;

    if (setsockopt(netsock->handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &option_value, sizeof option_value) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
