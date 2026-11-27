/**
 * \file socket_datagram_udp_internal.h
 * \brief Internal definition of the UDP sockets.
 *
 * A UDP socket wraps a system socket, so each of its operations calls the system socket wrapper.
 */

/**
 * \addtogroup SocketDatagram
 * @{
 */

#ifndef VMNL_NET_SOCKET_DATAGRAM_UDP_INTERNAL_H
#define VMNL_NET_SOCKET_DATAGRAM_UDP_INTERNAL_H

#include "socket/datagram/socket_datagram_internal.h"
#include "socket/system/socket_system_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief UDP socket, over a system socket.
 */
struct VmnlNetSocketDatagramUdp {
    VmnlNetSocketDatagram base; /*!< Common part, first so that a VmnlNetSocketDatagram pointer converts to it. */
    struct VmnlNetSocketSystem socket; /*!< System socket, of type SOCK_DGRAM. */
};

/**
 * \brief Creates a UDP socket.
 * \param ops Operations of UDP, stored in the socket.
 * \param version IP version of the socket, #VMNL_NET_ADDRESS_IPV4 or #VMNL_NET_ADDRESS_IPV6.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * - An error of vmnl_net_socket_system_open().
 * \return Created socket on success, NULL otherwise.
 * \remark Opens an AF_INET socket for IPv4 and an AF_INET6 one for IPv6, and frees the socket if the open fails.
 */
VmnlNetSocketDatagram *vmnl_net_socket_datagram_udp_create(const struct VmnlNetSocketDatagramOps *ops, VmnlNetAddressVersion version, VmnlNetError *error);

/**
 * \brief Destroys a UDP socket.
 * \param netsock Socket to destroy.
 * \remark Closes the system socket, then frees the socket.
 */
void vmnl_net_socket_datagram_udp_destroy(VmnlNetSocketDatagram *netsock);

/**
 * \brief Binds a UDP socket, with vmnl_net_socket_system_bind().
 * \param netsock Socket to bind.
 * \param address Local address.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_bind().
 * \return true on success, false otherwise.
 */
static inline bool vmnl_net_socket_datagram_udp_bind(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    return vmnl_net_socket_system_bind(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket, address, error);
}

/**
 * \brief Connects a UDP socket, with vmnl_net_socket_system_connect().
 * \param netsock Socket to connect.
 * \param address Address of the peer.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_connect().
 * \return true on success, false otherwise.
 */
static inline bool vmnl_net_socket_datagram_udp_connect(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    return vmnl_net_socket_system_connect(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket, address, error);
}

/**
 * \brief Sets the broadcast of a UDP socket, with vmnl_net_socket_system_set_broadcast().
 * \param netsock Socket to set.
 * \param enabled true to allow the broadcast, false to forbid it.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_set_broadcast().
 * \return true on success, false otherwise.
 */
static inline bool vmnl_net_socket_datagram_udp_set_broadcast(VmnlNetSocketDatagram *netsock, bool enabled, VmnlNetError *error)
{
    return vmnl_net_socket_system_set_broadcast(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket, enabled, error);
}

/**
 * \brief Sets the receive buffer size of a UDP socket, with vmnl_net_socket_system_set_receive_buffer_size().
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_set_receive_buffer_size().
 * \return true on success, false otherwise.
 */
static inline bool vmnl_net_socket_datagram_udp_set_receive_buffer_size(VmnlNetSocketDatagram *netsock, size_t size, VmnlNetError *error)
{
    return vmnl_net_socket_system_set_receive_buffer_size(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket, size, error);
}

/**
 * \brief Sets the send buffer size of a UDP socket, with vmnl_net_socket_system_set_send_buffer_size().
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_set_send_buffer_size().
 * \return true on success, false otherwise.
 */
static inline bool vmnl_net_socket_datagram_udp_set_send_buffer_size(VmnlNetSocketDatagram *netsock, size_t size, VmnlNetError *error)
{
    return vmnl_net_socket_system_set_send_buffer_size(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket, size, error);
}

/**
 * \brief Gets the local address of a UDP socket, with vmnl_net_socket_system_local_address().
 * \param netsock Socket to read.
 * \param address Local address, unchanged on failure.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_local_address().
 * \return true on success, false otherwise.
 */
static inline bool vmnl_net_socket_datagram_udp_local_address(const VmnlNetSocketDatagram *netsock, VmnlNetAddress *address, VmnlNetError *error)
{
    return vmnl_net_socket_system_local_address(&((const struct VmnlNetSocketDatagramUdp *) netsock)->socket, address, error);
}

/**
 * \brief Gets the system socket of a UDP socket.
 * \param netsock Socket to read.
 * \return System socket.
 */
static inline VmnlNetHandle vmnl_net_socket_datagram_udp_handle(const VmnlNetSocketDatagram *netsock)
{
    return ((const struct VmnlNetSocketDatagramUdp *) netsock)->socket.handle;
}

/**
 * \brief Waits until a UDP socket can receive or send, with vmnl_net_socket_system_wait_for().
 * \param netsock Socket to wait for.
 * \param events Events to wait for, known ones only.
 * \param timeout Nanoseconds to wait at most, 0 to check without waiting, #VMNL_NET_CLOCK_INF to wait without limit.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_wait_for().
 * \return Events of \p events that came, 0 on failure.
 */
static inline VmnlNetSocketEvents vmnl_net_socket_datagram_udp_wait_for(const VmnlNetSocketDatagram *netsock, VmnlNetSocketEvents events, uint64_t timeout, VmnlNetError *error)
{
    return vmnl_net_socket_system_wait_for(&((const struct VmnlNetSocketDatagramUdp *) netsock)->socket, events, timeout, error);
}

/**
 * \brief Sends one datagram on a UDP socket, with vmnl_net_socket_system_send_to().
 * \param netsock Socket to send on.
 * \param address Address of the recipient, NULL for the peer of a connected socket.
 * \param views Bytes of the datagram, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_send_to().
 * \return Number of bytes sent, 0 on failure.
 */
static inline size_t vmnl_net_socket_datagram_udp_send_to(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return vmnl_net_socket_system_send_to(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket, address, views, nviews, error);
}

/**
 * \brief Receives one datagram on a UDP socket, with vmnl_net_socket_system_receive_from().
 * \param netsock Socket to receive from.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param address Address of the sender, NULL to skip it.
 * \param error Error code of the call, nullable, an error of vmnl_net_socket_system_receive_from().
 * \return Number of bytes of the datagram, 0 on failure.
 */
static inline size_t vmnl_net_socket_datagram_udp_receive_from(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error)
{
    return vmnl_net_socket_system_receive_from(&((struct VmnlNetSocketDatagramUdp *) netsock)->socket, views, nviews, address, error);
}

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_SOCKET_DATAGRAM_UDP_INTERNAL_H */

/** @} */
