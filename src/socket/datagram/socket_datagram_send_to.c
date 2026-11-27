#include "socket/datagram/socket_datagram_internal.h"

size_t vmnl_net_socket_datagram_send_to(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return netsock->ops->send_to(netsock, address, views, nviews, error);
}
