#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_pending_error(struct VmnlNetSocket *netsock, VmnlNetError *error)
{
    int option_value = 0;

    if (getsockopt(netsock->handle, SOL_SOCKET, SO_ERROR, (char *) &option_value, &(socklen_t) {sizeof option_value}) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    if (option_value != 0) {
        vmnl_net_error_set(error, vmnl_net_error_from_system(option_value));
        return false;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
