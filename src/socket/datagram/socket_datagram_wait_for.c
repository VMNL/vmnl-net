#include "error/error_internal.h"
#include "socket/datagram/socket_datagram_internal.h"

VmnlNetSocketEvents vmnl_net_socket_datagram_wait_for(const VmnlNetSocketDatagram *netsock, VmnlNetSocketEvents events, uint64_t timeout, VmnlNetError *error)
{
    if (events == 0 || (events & ~(VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE)) != 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return 0;
    }
    return netsock->ops->wait_for(netsock, events, timeout, error);
}
