#include "allocator/allocator_internal.h"
#include "socket/datagram/udp/socket_datagram_udp_internal.h"

void vmnl_net_socket_datagram_udp_destroy(VmnlNetSocketDatagram *netsock)
{
    vmnl_net_socket_system_close(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket);
    vmnl_net_allocator.deallocate(netsock);
}
