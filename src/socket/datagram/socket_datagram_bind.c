#include "socket/datagram/socket_datagram_internal.h"

bool vmnl_net_socket_datagram_bind(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    return netsock->ops->bind(netsock, address, error);
}
