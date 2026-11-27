/**
 * \file datagram.h
 *
 * \defgroup SocketDatagram Datagram socket
 * \addtogroup SocketDatagram
 * \brief Sockets that send and receive whole datagrams, behind one API for every type, like UDP.
 *
 * A datagram socket is created with a type, like #VMNL_NET_SOCKET_DATAGRAM_UDP, and every function then works the same whatever the type.
 * A datagram is sent whole or not at all, and keeps its boundaries, but it may arrive out of order, twice or never.
 * A datagram socket never blocks, so a call that would wait fails with #VMNL_NET_EAGAIN.
 * An IPv6 socket also reaches IPv4 peers, through their address mapped as ::ffff:a.b.c.d.
 *
 * \par Types
 * The type is picked at creation, and an option that a type lacks fails with #VMNL_NET_ENOTSUP.
 *
 * \if MAN
 * The HTML documentation compares the types in a table.
 * \else
 * |                | UDP                                             |
 * |----------------|-------------------------------------------------|
 * | Network        | IPv4 or IPv6                                    |
 * | Broadcast      | Yes, through the mapped IPv4 broadcast for IPv6 |
 * | System buffers | Yes                                             |
 * \endif
 *
 * \par Creating a context
 * \attention A context **MUST** exist while a datagram socket is open, as the contexts start the sockets of Windows.
 * \snippet{trimleft} example_socket_datagram_basics.c create_context
 *
 * \par Creating a UDP socket
 * The IP version of the socket comes from its address, with vmnl_net_address_version().
 * \snippet{trimleft} example_socket_datagram_basics.c create_socket
 *
 * \par Sending a datagram
 * \snippet{trimleft} example_socket_datagram_basics.c send_datagram
 *
 * \par Receiving a datagram
 * \snippet{trimleft} example_socket_datagram_basics.c receive_datagram
 *
 * \par Destroying a socket
 * \snippet{trimleft} example_socket_datagram_basics.c destroy_socket
 *
 * \see \ref example_socket_datagram_basics.c
 * @{
 */

#ifndef VMNL_NET_SOCKET_DATAGRAM_H
#define VMNL_NET_SOCKET_DATAGRAM_H

#include <vmnl/net/address.h>
#include <vmnl/net/attribute.h>
#include <vmnl/net/buffer.h>
#include <vmnl/net/handle.h>
#include <vmnl/net/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque datagram socket, of any type.
 */
typedef struct VmnlNetSocketDatagram VmnlNetSocketDatagram;

/**
 * \brief Type of a datagram socket, see the types of the \ref SocketDatagram module.
 */
typedef uint32_t VmnlNetSocketDatagramType;

#define VMNL_NET_SOCKET_DATAGRAM_UDP UINT32_C(1) /*!< UDP over IPv4 or IPv6. */

/**
 * \brief Creates a datagram socket.
 * \param type Type of the socket, #VMNL_NET_SOCKET_DATAGRAM_UDP.
 * \param version IP version of the socket, #VMNL_NET_ADDRESS_IPV4 or #VMNL_NET_ADDRESS_IPV6.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p type or \p version is unknown.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * - #VMNL_NET_EAFNOSUPPORT: the host does not support \p version, like IPv6 without IPv6.
 * - #VMNL_NET_EMFILE: no descriptor is left for the process.
 * - #VMNL_NET_ENFILE: no file is left for the system, on POSIX.
 * - #VMNL_NET_ENOBUFS: the system lacks memory.
 * - #VMNL_NET_EACCES: the system denies the creation, on POSIX.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return Created socket on success, NULL otherwise.
 */
VMNL_NET_API VMNL_NET_NODISCARD VmnlNetSocketDatagram *vmnl_net_socket_datagram_create(VmnlNetSocketDatagramType type, VmnlNetAddressVersion version, VmnlNetError *error);

/**
 * \brief Binds a datagram socket to a local address.
 * \param netsock Socket to bind.
 * \param address Local address, with #VMNL_NET_ADDRESS_ANY_PORT to let the system choose the port.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAFNOSUPPORT: \p address is IPv6 and \p netsock is IPv4.
 * - #VMNL_NET_EADDRINUSE: another socket holds \p address.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address does not belong to the host.
 * - #VMNL_NET_EACCES: the port needs privileges, like a port below 1024 on Linux.
 * - #VMNL_NET_EINVAL: \p netsock is already bound.
 * - #VMNL_NET_ENOBUFS: the system lacks buffers, on Windows.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_socket_datagram_bind(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Sets the peer of a datagram socket.
 *
 * vmnl_net_socket_datagram_send() then sends to the peer, and the socket only receives its datagrams.
 * A datagram sent to a closed port makes a later receive fail with #VMNL_NET_ECONNREFUSED, even after a connect to another peer, which an unconnected socket never reports.
 * \param netsock Socket to connect.
 * \param address Address of the peer.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAFNOSUPPORT: \p address is IPv6 and \p netsock is IPv4.
 * - #VMNL_NET_EACCES: \p address is 255.255.255.255 without vmnl_net_socket_datagram_set_broadcast(), or a directed broadcast address like 192.168.1.255 without it on Linux.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address is a wildcard like 0.0.0.0 on Windows, or is not available on macOS.
 * - #VMNL_NET_ENETUNREACH or #VMNL_NET_EHOSTUNREACH: no route reaches \p address.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_socket_datagram_connect(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Lets a datagram socket send to a broadcast address, like 255.255.255.255.
 * \param netsock Socket to set.
 * \param enabled true to allow the broadcast, false to forbid it.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOTSUP: the type of \p netsock has no broadcast.
 * - #VMNL_NET_ENOBUFS: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark An IPv6 socket broadcasts to IPv4 peers through ::ffff:255.255.255.255, as IPv6 itself has no broadcast.
 */
VMNL_NET_API bool vmnl_net_socket_datagram_set_broadcast(VmnlNetSocketDatagram *netsock, bool enabled, VmnlNetError *error);

/**
 * \brief Sets the size of the system's receive buffer of a datagram socket.
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOTSUP: the type of \p netsock has no system buffer.
 * - #VMNL_NET_EINVAL: \p size is greater than INT_MAX, or \p size is 0 on macOS.
 * - #VMNL_NET_ENOBUFS: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Linux doubles the size, for its own bookkeeping.
 */
VMNL_NET_API bool vmnl_net_socket_datagram_set_receive_buffer_size(VmnlNetSocketDatagram *netsock, size_t size, VmnlNetError *error);

/**
 * \brief Sets the size of the system's send buffer of a datagram socket.
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOTSUP: the type of \p netsock has no system buffer.
 * - #VMNL_NET_EINVAL: \p size is greater than INT_MAX, or \p size is 0 on macOS.
 * - #VMNL_NET_ENOBUFS: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Linux doubles the size, for its own bookkeeping.
 */
VMNL_NET_API bool vmnl_net_socket_datagram_set_send_buffer_size(VmnlNetSocketDatagram *netsock, size_t size, VmnlNetError *error);

/**
 * \brief Gets the local address of a datagram socket.
 * \param netsock Socket to read.
 * \param address Local address, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p netsock is not bound, on Windows.
 * - #VMNL_NET_ENOBUFS: the system lacks resources, on POSIX.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Gives the port the system chose after a bind to #VMNL_NET_ADDRESS_ANY_PORT.
 */
VMNL_NET_API bool vmnl_net_socket_datagram_local_address(const VmnlNetSocketDatagram *netsock, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Gets the system socket of a datagram socket.
 * \param netsock Socket to read.
 * \return System socket, to pass to the system's own functions.
 * \attention
 * - The system socket **MUST** not be closed, as \p netsock owns it and closes it with vmnl_net_socket_datagram_destroy().
 * - Its broadcast option **MUST** only change through vmnl_net_socket_datagram_set_broadcast(), as the library keeps it to refuse a broadcast without it on every system.
 */
VMNL_NET_API VmnlNetHandle vmnl_net_socket_datagram_handle(const VmnlNetSocketDatagram *netsock);

/**
 * \brief Waits until a datagram socket can receive or send.
 *
 * A datagram waiting makes the socket readable, and room in the system's send buffer makes it writable.
 * An error waiting for the next call makes the socket ready for every event asked, so the next receive or send reports it.
 * A signal handled by the application does not end the wait, which goes on for the time left.
 * \param netsock Socket to wait for.
 * \param events Events to wait for, #VMNL_NET_SOCKET_READABLE, #VMNL_NET_SOCKET_WRITABLE or both.
 * \param timeout Nanoseconds to wait at most, 0 to check without waiting, #VMNL_NET_CLOCK_INF to wait without limit.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ETIMEDOUT: none of \p events came within \p timeout.
 * - #VMNL_NET_EINVAL: \p events is 0 or holds an unknown event.
 * - #VMNL_NET_ENOMEM or #VMNL_NET_ENOBUFS: the system lacks memory.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return Events of \p events that came, 0 on failure.
 */
VMNL_NET_API VmnlNetSocketEvents vmnl_net_socket_datagram_wait_for(const VmnlNetSocketDatagram *netsock, VmnlNetSocketEvents events, uint64_t timeout, VmnlNetError *error);

/**
 * \brief Sends one datagram to the peer of a connected datagram socket.
 * \param netsock Socket to send on, connected with vmnl_net_socket_datagram_connect().
 * \param views Bytes of the datagram, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOTCONN: \p netsock is not connected.
 * - #VMNL_NET_EMSGSIZE: the datagram is too large for the socket, or \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_EAGAIN: the system's send buffer is full.
 * - #VMNL_NET_ENETUNREACH or #VMNL_NET_EHOSTUNREACH: no route reaches the peer.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - #VMNL_NET_ENETRESET: the time to live expired, on Windows.
 * - #VMNL_NET_EINVAL: \p nviews is 0, or \p views hold more than INT_MAX bytes on macOS.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory.
 * - Any other translated system error.
 * \return Number of bytes sent, the whole datagram, 0 with #VMNL_NET_SUCCESS for an empty datagram, 0 on failure.
 */
VMNL_NET_API size_t vmnl_net_socket_datagram_send(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error);

/**
 * \brief Sends one datagram to an address.
 * \param netsock Socket to send on.
 * \param address Address of the recipient.
 * \param views Bytes of the datagram, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAFNOSUPPORT: \p address is IPv6 and \p netsock is IPv4.
 * - #VMNL_NET_EMSGSIZE: the datagram is too large for the socket, or \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_EAGAIN: the system's send buffer is full, or no ephemeral port is left for an unbound socket on Linux.
 * - #VMNL_NET_EACCES: \p address is 255.255.255.255 without vmnl_net_socket_datagram_set_broadcast(), or a directed broadcast address like 192.168.1.255 without it or a prohibit route reaches it on Linux.
 * - #VMNL_NET_ENETUNREACH or #VMNL_NET_EHOSTUNREACH: no route reaches \p address.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - #VMNL_NET_ENETRESET: the time to live expired, on Windows.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address is a wildcard like 0.0.0.0 on Windows, or the local address is no longer available on macOS.
 * - #VMNL_NET_EISCONN: \p netsock is connected.
 * - #VMNL_NET_EINVAL: \p nviews is 0, \p views hold more than INT_MAX bytes on macOS, or a blackhole route reaches \p address on Linux.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory.
 * - Any other translated system error.
 * \return Number of bytes sent, the whole datagram, 0 with #VMNL_NET_SUCCESS for an empty datagram, 0 on failure.
 */
VMNL_NET_API size_t vmnl_net_socket_datagram_send_to(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error);

/**
 * \brief Receives one datagram from the peer of a connected datagram socket.
 * \param netsock Socket to receive from.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: no datagram is waiting.
 * - #VMNL_NET_EMSGSIZE: the datagram was larger than \p views, and its end is lost, or \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_ECONNREFUSED: an earlier datagram found no receiver, on a connected socket.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - #VMNL_NET_EINVAL: \p nviews is 0, \p netsock is not bound on Windows, or \p views hold more than INT_MAX bytes on macOS.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on POSIX.
 * - Any other translated system error.
 * \return Number of bytes of the datagram, 0 with #VMNL_NET_SUCCESS for an empty datagram, 0 on failure.
 */
VMNL_NET_API size_t vmnl_net_socket_datagram_receive(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error);

/**
 * \brief Receives one datagram and the address of its sender.
 * \param netsock Socket to receive from.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param address Address of the sender, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: no datagram is waiting.
 * - #VMNL_NET_EMSGSIZE: the datagram was larger than \p views, and its end is lost, or \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_ECONNREFUSED: an earlier datagram found no receiver, on a connected socket.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - #VMNL_NET_EINVAL: \p nviews is 0, \p netsock is not bound on Windows, or \p views hold more than INT_MAX bytes on macOS.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on POSIX.
 * - Any other translated system error.
 * \return Number of bytes of the datagram, 0 with #VMNL_NET_SUCCESS for an empty datagram, 0 on failure.
 */
VMNL_NET_API size_t vmnl_net_socket_datagram_receive_from(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Destroys a datagram socket and closes its system socket.
 * \param netsock Socket to destroy, nullable.
 */
VMNL_NET_API void vmnl_net_socket_datagram_destroy(VmnlNetSocketDatagram *netsock);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_SOCKET_DATAGRAM_H */

/** @} */

/**
 * \example example_socket_datagram_basics.c
 * Creates two UDP sockets, sends a datagram from one to the other and receives it.
 */
