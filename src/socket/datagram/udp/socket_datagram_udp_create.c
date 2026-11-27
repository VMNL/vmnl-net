#include "allocator/allocator_internal.h"
#include "socket/datagram/udp/socket_datagram_udp_internal.h"

VmnlNetSocketDatagram *vmnl_net_socket_datagram_udp_create(const struct VmnlNetSocketDatagramOps *ops, VmnlNetAddressVersion version, VmnlNetError *error)
{
    struct VmnlNetSocketDatagramUdp *netsock = vmnl_net_allocator.allocate(sizeof *netsock);

    if (netsock == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    if (vmnl_net_socket_system_open((version == VMNL_NET_ADDRESS_IPV4) ? AF_INET : AF_INET6, SOCK_DGRAM, &netsock->socket, error) == false) {
        vmnl_net_allocator.deallocate(netsock);
        return NULL;
    }
    netsock->base.ops = ops;
    return &netsock->base;
}
