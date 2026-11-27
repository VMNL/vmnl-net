#include "socket/datagram/socket_datagram_internal.h"

VmnlNetHandle vmnl_net_socket_datagram_handle(const VmnlNetSocketDatagram *netsock)
{
    return netsock->ops->handle(netsock);
}
