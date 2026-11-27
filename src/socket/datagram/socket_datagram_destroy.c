#include "socket/datagram/socket_datagram_internal.h"

void vmnl_net_socket_datagram_destroy(VmnlNetSocketDatagram *netsock)
{
    if (netsock == NULL) {
        return;
    }
    netsock->ops->destroy(netsock);
}
