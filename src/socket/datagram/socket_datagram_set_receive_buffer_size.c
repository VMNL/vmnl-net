#include "error/error_internal.h"
#include "socket/datagram/socket_datagram_internal.h"

bool vmnl_net_socket_datagram_set_receive_buffer_size(VmnlNetSocketDatagram *netsock, size_t size, VmnlNetError *error)
{
    if (netsock->ops->set_receive_buffer_size == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOTSUP);
        return false;
    }
    return netsock->ops->set_receive_buffer_size(netsock, size, error);
}
