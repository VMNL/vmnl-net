#include "socket/datagram/socket_datagram_internal.h"

bool vmnl_net_socket_datagram_local_address(const VmnlNetSocketDatagram *netsock, VmnlNetAddress *address, VmnlNetError *error)
{
    return netsock->ops->local_address(netsock, address, error);
}
