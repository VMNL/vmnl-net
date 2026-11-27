#include "socket/datagram/socket_datagram_internal.h"

size_t vmnl_net_socket_datagram_receive_from(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error)
{
    return netsock->ops->receive_from(netsock, views, nviews, address, error);
}
