#include "socket/datagram/socket_datagram_internal.h"

bool vmnl_net_socket_datagram_connect(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    return netsock->ops->connect(netsock, address, error);
}
