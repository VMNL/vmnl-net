#include "error/error_internal.h"
#include "socket/datagram/socket_datagram_internal.h"
#include "socket/datagram/udp/socket_datagram_udp_internal.h"

static const struct VmnlNetSocketDatagramOps vmnl_net_socket_datagram_udp_ops = {
    .create = vmnl_net_socket_datagram_udp_create,
    .bind = vmnl_net_socket_datagram_udp_bind,
    .connect = vmnl_net_socket_datagram_udp_connect,
    .set_broadcast = vmnl_net_socket_datagram_udp_set_broadcast,
    .set_receive_buffer_size = vmnl_net_socket_datagram_udp_set_receive_buffer_size,
    .set_send_buffer_size = vmnl_net_socket_datagram_udp_set_send_buffer_size,
    .local_address = vmnl_net_socket_datagram_udp_local_address,
    .handle = vmnl_net_socket_datagram_udp_handle,
    .wait_for = vmnl_net_socket_datagram_udp_wait_for,
    .send_to = vmnl_net_socket_datagram_udp_send_to,
    .receive_from = vmnl_net_socket_datagram_udp_receive_from,
    .destroy = vmnl_net_socket_datagram_udp_destroy,
};

static const struct VmnlNetSocketDatagramOps *const vmnl_net_socket_datagram_ops[] = {
    [VMNL_NET_SOCKET_DATAGRAM_UDP] = &vmnl_net_socket_datagram_udp_ops,
};

VmnlNetSocketDatagram *vmnl_net_socket_datagram_create(VmnlNetSocketDatagramType type, VmnlNetAddressVersion version, VmnlNetError *error)
{
    if (type >= sizeof vmnl_net_socket_datagram_ops / sizeof vmnl_net_socket_datagram_ops[0] || vmnl_net_socket_datagram_ops[type] == NULL || (version != VMNL_NET_ADDRESS_IPV4 && version != VMNL_NET_ADDRESS_IPV6)) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    return vmnl_net_socket_datagram_ops[type]->create(vmnl_net_socket_datagram_ops[type], version, error);
}
