#include "socket/datagram/socket_datagram_internal.h"

size_t vmnl_net_socket_datagram_send(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return netsock->ops->send_to(netsock, NULL, views, nviews, error);
}
