#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_set_linger(struct VmnlNetSocketSystem *netsock, bool enabled, uint16_t seconds, VmnlNetError *error)
{
    struct linger option_value = {.l_onoff = enabled, .l_linger = seconds};

    if (setsockopt(netsock->handle, SOL_SOCKET, SO_LINGER, (const char *) &option_value, sizeof option_value) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
