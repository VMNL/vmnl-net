#include "socket/datagram/socket_datagram_internal.h"

size_t vmnl_net_socket_datagram_receive(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return netsock->ops->receive_from(netsock, views, nviews, NULL, error);
}
