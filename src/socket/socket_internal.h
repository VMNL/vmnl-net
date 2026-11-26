/**
 * \file socket_internal.h
 * \defgroup Socket Socket
 * \addtogroup Socket
 * \brief Internal wrapper over the BSD sockets and Winsock2.
 *
 * The wrapper merges the BSD sockets and Winsock2 into one set of functions, which each kind with a system socket reuses instead of calling the systems itself.
 * A socket is always non-blocking and never inherited by child processes. An IPv6 socket also reaches IPv4 peers.
 * A system failure is translated into a code of the library through vmnl_net_error_from_last_system().
 * @{
 */

#ifndef VMNL_NET_SOCKET_INTERNAL_H
#define VMNL_NET_SOCKET_INTERNAL_H

#include <vmnl/net/address.h> /* IWYU pragma: export */
#include <vmnl/net/buffer.h> /* IWYU pragma: export */
#include <vmnl/net/error.h> /* IWYU pragma: export */
#include <vmnl/net/handle.h> /* IWYU pragma: export */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * \def VMNL_NET_SOCKET_INVALID
 * \brief Handle that socket() and accept() return on failure.
 * \remark INVALID_SOCKET on Windows, -1 elsewhere.
 */
/**
 * \def VMNL_NET_SOCKET_SHUTDOWN_SEND
 * \brief Direction that vmnl_net_socket_shutdown() closes.
 * \remark SD_SEND on Windows, SHUT_WR elsewhere.
 */
#ifdef _WIN32
#include <winsock2.h> /* IWYU pragma: export */
#include <ws2tcpip.h> /* IWYU pragma: export */

#define VMNL_NET_SOCKET_INVALID       INVALID_SOCKET
#define VMNL_NET_SOCKET_SHUTDOWN_SEND SD_SEND

_Static_assert(sizeof(VmnlNetBufferView) == sizeof(WSABUF), "a VmnlNetBufferView must have the layout of a WSABUF");
_Static_assert(offsetof(VmnlNetBufferView, size) == offsetof(WSABUF, len), "a VmnlNetBufferView must have the layout of a WSABUF");
_Static_assert(offsetof(VmnlNetBufferView, data) == offsetof(WSABUF, buf), "a VmnlNetBufferView must have the layout of a WSABUF");
#else
#include <netinet/in.h> /* IWYU pragma: export */
#include <netinet/tcp.h> /* IWYU pragma: export */
#include <sys/socket.h> /* IWYU pragma: export */

#define VMNL_NET_SOCKET_INVALID       (-1)
#define VMNL_NET_SOCKET_SHUTDOWN_SEND SHUT_WR

_Static_assert(sizeof(VmnlNetBufferView) == sizeof(struct iovec), "a VmnlNetBufferView must have the layout of a struct iovec");
_Static_assert(offsetof(VmnlNetBufferView, data) == offsetof(struct iovec, iov_base), "a VmnlNetBufferView must have the layout of a struct iovec");
_Static_assert(offsetof(VmnlNetBufferView, size) == offsetof(struct iovec, iov_len), "a VmnlNetBufferView must have the layout of a struct iovec");
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief System socket, its address family and its type.
 * \remark Embedded by value in each kind with a system socket, like TCP or UDP.
 */
struct VmnlNetSocket {
    VmnlNetHandle handle; /*!< System socket, a SOCKET on Windows and a file descriptor elsewhere. */
    int family; /*!< AF_INET or AF_INET6, used to convert the addresses passed to the socket. */
    int type; /*!< SOCK_STREAM or SOCK_DGRAM, used to handle the ICMP errors of a datagram socket on Windows. */
};

#ifdef _WIN32
#include <mswsock.h> /* IWYU pragma: export */

/**
 * \brief Sets whether a datagram socket reports an ICMP error that an earlier datagram drew, on Windows.
 * \param netsock Datagram socket to set.
 * \param control SIO_UDP_CONNRESET for a port unreachable, reported with WSAECONNRESET, or SIO_UDP_NETRESET for an expired time to live, reported with WSAENETRESET.
 * \param enabled true to report the error, false to drop it.
 * \return true on success, false otherwise, with the error in WSAGetLastError().
 * \remark Windows reports both errors on any datagram socket by default. POSIX reports a port unreachable on a connected socket only, so vmnl_net_socket_open() clears both and vmnl_net_socket_connect() sets SIO_UDP_CONNRESET again.
 */
static inline bool vmnl_net_socket_report_unreachable(const struct VmnlNetSocket *netsock, DWORD control, BOOL enabled)
{
    return WSAIoctl(netsock->handle, control, &enabled, sizeof enabled, NULL, 0, &(DWORD) {0}, NULL, NULL) == 0;
}
#endif

/**
 * \brief Opens a system socket, non-blocking and not inherited by child processes.
 * \param family AF_INET or AF_INET6.
 * \param type SOCK_STREAM or SOCK_DGRAM.
 * \param netsock Socket opened, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAFNOSUPPORT: the host does not support \p family, like AF_INET6 without IPv6.
 * - #VMNL_NET_EMFILE: no descriptor is left for the process.
 * - #VMNL_NET_ENFILE: no file is left for the system, on POSIX.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory.
 * - #VMNL_NET_EACCES: the system denies the creation, on POSIX.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - An error of vmnl_net_socket_non_blocking() or vmnl_net_socket_dual_stack(), or of vmnl_net_socket_no_inheritance() on POSIX.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Calls vmnl_net_socket_dual_stack() on an AF_INET6 socket.
 * \remark Creates the socket not inherited with WSA_FLAG_NO_HANDLE_INHERIT on Windows, and calls vmnl_net_socket_no_inheritance() on POSIX.
 * \remark Clears SIO_UDP_CONNRESET and SIO_UDP_NETRESET with vmnl_net_socket_report_unreachable() on a datagram socket, on Windows.
 * \remark Closes the socket if one of its settings fails.
 * \attention A context **MUST** exist while the socket is open, as the contexts start Winsock on Windows.
 */
bool vmnl_net_socket_open(int family, int type, struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Makes a socket non-blocking.
 *
 * A call that would wait then fails with #VMNL_NET_EAGAIN.
 * \param netsock Socket to set.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets O_NONBLOCK with fcntl() on POSIX, and FIONBIO with ioctlsocket() on Windows.
 */
bool vmnl_net_socket_non_blocking(struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Keeps a socket out of the child processes.
 * \param netsock Socket to set.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_FSYSTEM: SetHandleInformation() failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets FD_CLOEXEC with fcntl() on POSIX, and clears HANDLE_FLAG_INHERIT with SetHandleInformation() on Windows.
 */
bool vmnl_net_socket_no_inheritance(struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Lets a listener rebind a port still in TIME_WAIT.
 * \param netsock Socket to set.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_REUSEADDR on POSIX. Windows gets SO_EXCLUSIVEADDRUSE instead, as its SO_REUSEADDR lets another socket take a port in use.
 * \attention It **MUST** be called before vmnl_net_socket_bind().
 */
bool vmnl_net_socket_reuse_address(struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Lets an IPv6 socket also reach IPv4 peers, through their address mapped as ::ffff:a.b.c.d.
 * \param netsock Socket to set.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Clears IPV6_V6ONLY, as its default differs between systems.
 * \attention \p netsock **MUST** be an AF_INET6 socket, as an AF_INET socket rejects the option.
 */
bool vmnl_net_socket_dual_stack(struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Sends the small writes of a stream socket at once, instead of grouping them.
 * \param netsock Socket to set.
 * \param enabled true to send at once, false to let the system group the writes with Nagle's algorithm.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: the peer reset the connection, on macOS.
 * - #VMNL_NET_ENETRESET: the connection timed out with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOTCONN: the connection was reset with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets TCP_NODELAY.
 * \attention \p netsock **MUST** be a stream socket.
 */
bool vmnl_net_socket_set_no_delay(struct VmnlNetSocket *netsock, bool enabled, VmnlNetError *error);

/**
 * \brief Sends probes on an idle stream socket, to detect a peer that disappeared.
 * \param netsock Socket to set.
 * \param enabled true to send the probes, false to stop them.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: the peer reset the connection, on macOS.
 * - #VMNL_NET_ENETRESET: the connection timed out with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOTCONN: the connection was reset with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_KEEPALIVE, with the system's delays.
 */
bool vmnl_net_socket_set_keep_alive(struct VmnlNetSocket *netsock, bool enabled, VmnlNetError *error);

/**
 * \brief Sets how long closing a stream socket waits for unsent bytes.
 * \param netsock Socket to set.
 * \param enabled true to wait for the bytes up to \p seconds, false to let the system send them after the close.
 * \param seconds Seconds to wait when \p enabled is true, 0 to drop the bytes and reset the connection.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: the peer reset the connection, on macOS.
 * - #VMNL_NET_ENETRESET: the connection timed out with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOTCONN: the connection was reset with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_LINGER. macOS counts it in seconds too, as the library builds with _POSIX_C_SOURCE.
 */
bool vmnl_net_socket_set_linger(struct VmnlNetSocket *netsock, bool enabled, uint16_t seconds, VmnlNetError *error);

/**
 * \brief Sets the size of the system's receive buffer.
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p size is greater than INT_MAX, \p size is 0 on macOS, or the peer reset the connection on macOS.
 * - #VMNL_NET_ENETRESET: the connection timed out with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOTCONN: the connection was reset with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_RCVBUF. Linux doubles the size, for its own bookkeeping.
 */
bool vmnl_net_socket_set_receive_buffer_size(struct VmnlNetSocket *netsock, size_t size, VmnlNetError *error);

/**
 * \brief Sets the size of the system's send buffer.
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p size is greater than INT_MAX, \p size is 0 on macOS, or the peer reset the connection on macOS.
 * - #VMNL_NET_ENETRESET: the connection timed out with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOTCONN: the connection was reset with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_SNDBUF. Linux doubles the size, for its own bookkeeping.
 */
bool vmnl_net_socket_set_send_buffer_size(struct VmnlNetSocket *netsock, size_t size, VmnlNetError *error);

/**
 * \brief Lets a datagram socket send to a broadcast address, like 255.255.255.255.
 * \param netsock Socket to set.
 * \param enabled true to allow the broadcast, false to forbid it.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_BROADCAST. IPv6 has no broadcast.
 */
bool vmnl_net_socket_set_broadcast(struct VmnlNetSocket *netsock, bool enabled, VmnlNetError *error);

/**
 * \brief Reads and clears the pending error of a socket.
 * \param netsock Socket to read.
 * \param error Error code of the call, nullable:
 * - The translated pending code, like an error of vmnl_net_socket_connect() after a failed connection.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true if no error is pending, false otherwise.
 * \remark Reads SO_ERROR.
 */
bool vmnl_net_socket_pending_error(struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Gets the local address of a socket.
 * \param netsock Socket to read.
 * \param address Local address, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p netsock is not bound, on Windows.
 * - #VMNL_NET_ENOBUFS: the system lacks resources, on POSIX.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Calls getsockname(), which gives the port the system chose after a bind to port 0.
 */
bool vmnl_net_socket_local_address(const struct VmnlNetSocket *netsock, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Binds a socket to a local address.
 * \param netsock Socket to bind.
 * \param address Local address, port 0 to let the system choose one.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAFNOSUPPORT: \p address is an IPv6 address and \p netsock an AF_INET socket.
 * - #VMNL_NET_EADDRINUSE: another socket holds \p address, or no ephemeral port is left for port 0 on Linux.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address does not belong to the host.
 * - #VMNL_NET_EACCES: the port needs privileges, like a port below 1024 on Linux, or \p address is a broadcast address without vmnl_net_socket_set_broadcast() on Windows.
 * - #VMNL_NET_EINVAL: \p netsock is already bound, or a link-local \p address has scope 0 on Linux or a scope naming no interface on macOS.
 * - #VMNL_NET_FSYSTEM: a link-local \p address has a scope naming no interface, on Linux.
 * - #VMNL_NET_ENOBUFS: the system lacks buffers, on Windows.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Converts \p address for the family of \p netsock, an IPv4 address staying mapped on an AF_INET6 socket.
 */
bool vmnl_net_socket_bind(struct VmnlNetSocket *netsock, const VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Makes a stream socket accept connections.
 * \param netsock Socket to listen on.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EADDRINUSE: another socket listens on the same port, or no ephemeral port is left for an unbound socket on Linux.
 * - #VMNL_NET_EINVAL: \p netsock is not bound on Windows, or already connected on macOS.
 * - #VMNL_NET_EISCONN: \p netsock is already connected, on Windows.
 * - #VMNL_NET_EACCES: the process lacks privileges, on macOS.
 * - #VMNL_NET_EMFILE: no descriptor is left, on Windows.
 * - #VMNL_NET_ENOBUFS: the system lacks buffers, on Windows.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Asks for a backlog of SOMAXCONN, which the system caps with its own limit, like somaxconn on Linux.
 * \attention \p netsock **MUST** be a stream socket.
 */
bool vmnl_net_socket_listen(struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Accepts a connection waiting on a listening socket.
 * \param netsock Listening socket.
 * \param peer Accepted socket, unchanged on failure.
 * \param address Address of the peer, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: no connection is waiting.
 * - #VMNL_NET_ECONNABORTED: a waiting connection was aborted, on POSIX.
 * - #VMNL_NET_ECONNRESET: the peer closed a waiting connection, on Windows.
 * - #VMNL_NET_EINVAL: \p netsock is not listening.
 * - #VMNL_NET_EMFILE: no descriptor is left for the process.
 * - #VMNL_NET_ENFILE: no file is left for the system, on POSIX.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory.
 * - #VMNL_NET_EPERM: a firewall rule forbids the connection, on Linux.
 * - #VMNL_NET_ENETDOWN, #VMNL_NET_ENETUNREACH, #VMNL_NET_EHOSTUNREACH, #VMNL_NET_ENOTSUP, #VMNL_NET_ETIMEDOUT or #VMNL_NET_FSYSTEM: a network error was pending on the new connection, on Linux, which advises a retry like #VMNL_NET_EAGAIN.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - An error of vmnl_net_socket_non_blocking() or vmnl_net_socket_no_inheritance().
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Makes the accepted socket non-blocking and not inherited, and closes it if one of them fails. Linux does not copy O_NONBLOCK from the listening socket.
 * \remark The accepted socket gets the peer's family, AF_INET6 for an IPv4 peer of an AF_INET6 listener.
 */
bool vmnl_net_socket_accept(struct VmnlNetSocket *netsock, struct VmnlNetSocket *peer, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Starts a connection to a remote address.
 * \param netsock Socket to connect.
 * \param address Remote address.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINPROGRESS: the connection started and completes later.
 * - #VMNL_NET_EAFNOSUPPORT: \p address is an IPv6 address and \p netsock an AF_INET socket.
 * - #VMNL_NET_EALREADY: a previous connection has not completed yet.
 * - #VMNL_NET_EISCONN: \p netsock is already connected, or listening on Linux.
 * - #VMNL_NET_ECONNREFUSED: no socket listens at \p address.
 * - #VMNL_NET_ETIMEDOUT: the connection timed out.
 * - #VMNL_NET_ENETUNREACH: no network reaches \p address.
 * - #VMNL_NET_EHOSTUNREACH: no route reaches \p address.
 * - #VMNL_NET_ECONNRESET: the remote host reset the connection, on macOS.
 * - #VMNL_NET_EACCES: \p address is a broadcast address without vmnl_net_socket_set_broadcast(), or a firewall or security rule forbids the connection on Linux.
 * - #VMNL_NET_EPERM: the same cases as #VMNL_NET_EACCES, on Linux.
 * - #VMNL_NET_EADDRINUSE: the local address is already in use.
 * - #VMNL_NET_EADDRNOTAVAIL: no ephemeral port is left on Linux, \p address is not available on macOS, or \p address is a wildcard like 0.0.0.0 on Windows.
 * - #VMNL_NET_EAGAIN: the routing cache lacks entries, on Linux.
 * - #VMNL_NET_EINVAL: \p netsock is listening, or a previous connection has not completed yet, on Windows.
 * - #VMNL_NET_ENOTSUP: \p netsock is listening, on macOS.
 * - #VMNL_NET_ENOBUFS: the system lacks buffers, on macOS and Windows.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - Any other translated system error.
 * \return true if the connection completed at once, false otherwise.
 * \remark Windows reports a connection in progress with WSAEWOULDBLOCK. It gives #VMNL_NET_EINPROGRESS here, where vmnl_net_error_from_system() gives #VMNL_NET_EAGAIN.
 * \remark A stream socket becomes writable once the connection completes, and vmnl_net_socket_pending_error() then gives its result.
 * \remark Sets SIO_UDP_CONNRESET again with vmnl_net_socket_report_unreachable() once a datagram socket is connected, on Windows.
 */
bool vmnl_net_socket_connect(struct VmnlNetSocket *netsock, const VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Sends bytes on a connected socket.
 * \param netsock Socket to send on.
 * \param views Bytes to send, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: the system's send buffer is full.
 * - #VMNL_NET_EPIPE: vmnl_net_socket_shutdown() stopped sending, or the connection is closed on POSIX, a reset included on macOS.
 * - #VMNL_NET_ECONNRESET: the peer reset the connection.
 * - #VMNL_NET_ETIMEDOUT: the peer stopped acknowledging the data, on Linux.
 * - #VMNL_NET_ECONNABORTED: the connection failed, like on a timeout, on Windows.
 * - #VMNL_NET_ENETRESET: keep-alive detected a failure, on Windows.
 * - #VMNL_NET_ENOTCONN: \p netsock is not connected.
 * - #VMNL_NET_ENETUNREACH or #VMNL_NET_EHOSTUNREACH: no route reaches the peer, on macOS.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - #VMNL_NET_EADDRNOTAVAIL: the local address is no longer available, on macOS.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, or \p views are too large on Windows, like 2 GiB.
 * - #VMNL_NET_EMSGSIZE: \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_EINVAL: \p nviews is 0, or \p views hold more than INT_MAX bytes on macOS.
 * - Any other translated system error.
 * \return Number of bytes sent, which can be less than \p views holds, 0 on failure.
 * \remark Never raises SIGPIPE.
 */
size_t vmnl_net_socket_send(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error);

/**
 * \brief Receives bytes from a connected socket.
 * \param netsock Socket to receive from.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: no byte is waiting.
 * - #VMNL_NET_ECONNRESET: the peer reset the connection.
 * - #VMNL_NET_ECONNREFUSED: the peer refused the connection, on Linux.
 * - #VMNL_NET_ETIMEDOUT: the connection timed out.
 * - #VMNL_NET_ECONNABORTED: the connection failed, like on a timeout, on Windows.
 * - #VMNL_NET_ENETRESET: keep-alive detected a failure, on Windows.
 * - #VMNL_NET_ENOTCONN: \p netsock is not connected.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on POSIX.
 * - #VMNL_NET_EMSGSIZE: \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_EINVAL: \p nviews is 0, or \p views hold more than INT_MAX bytes on macOS.
 * - Any other translated system error.
 * \return Number of bytes received, 0 with #VMNL_NET_SUCCESS when the peer closed the connection, 0 on failure.
 */
size_t vmnl_net_socket_receive(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error);

/**
 * \brief Sends one datagram to an address.
 *
 * A datagram is sent whole or not at all.
 * \param netsock Socket to send on, a datagram socket unless \p address is NULL.
 * \param address Address of the recipient, NULL for a connected socket.
 * \param views Bytes of the datagram, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAFNOSUPPORT: \p address is an IPv6 address and \p netsock an AF_INET socket.
 * - #VMNL_NET_EMSGSIZE: the datagram is too large for the socket, or \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_EAGAIN: the system's send buffer is full, or no ephemeral port is left for an unbound socket on Linux.
 * - #VMNL_NET_EACCES: \p address is a broadcast address without vmnl_net_socket_set_broadcast(), or a prohibit route reaches it on Linux.
 * - #VMNL_NET_ECONNREFUSED: an earlier datagram found no receiver, on a connected socket.
 * - #VMNL_NET_ENETUNREACH or #VMNL_NET_EHOSTUNREACH: no route reaches \p address.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - #VMNL_NET_ENETRESET: the time to live expired, on Windows.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address is a wildcard like 0.0.0.0 on Windows, or the local address is no longer available on macOS.
 * - #VMNL_NET_EISCONN: \p address is not NULL and \p netsock is connected, on macOS.
 * - #VMNL_NET_ENOTCONN: \p address is NULL and \p netsock is not connected.
 * - #VMNL_NET_EPIPE: vmnl_net_socket_shutdown() stopped sending on \p netsock.
 * - #VMNL_NET_EINVAL: \p nviews is 0, \p views hold more than INT_MAX bytes on macOS, or a blackhole route reaches \p address on Linux.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory.
 * - Any other translated system error.
 * \return Number of bytes sent, the whole datagram on a datagram socket, 0 on failure.
 * \remark With a NULL \p address on a stream socket, it behaves like vmnl_net_socket_send(), whose errors apply.
 */
size_t vmnl_net_socket_send_to(struct VmnlNetSocket *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error);

/**
 * \brief Receives one datagram and the address of its sender.
 * \param netsock Socket to receive from, a datagram socket unless \p address is NULL.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param address Address of the sender, NULL to skip it, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: no datagram is waiting.
 * - #VMNL_NET_EMSGSIZE: the datagram was larger than \p views, and its end is lost, or \p nviews is more than IOV_MAX on POSIX.
 * - #VMNL_NET_ECONNREFUSED: an earlier datagram found no receiver, on a connected socket.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - #VMNL_NET_EINVAL: \p nviews is 0, \p netsock is not bound on Windows, or \p views hold more than INT_MAX bytes on macOS.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on POSIX.
 * - Any other translated system error.
 * \return Number of bytes of the datagram, 0 with #VMNL_NET_SUCCESS for an empty datagram, 0 on failure.
 * \remark With a NULL \p address on a stream socket, it behaves like vmnl_net_socket_receive(), whose errors apply.
 */
size_t vmnl_net_socket_receive_from(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Stops sending on a connected socket, so the peer receives the end of the stream after the bytes already sent.
 * \param netsock Socket to shut down.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOTCONN: the sending side is already shut down, on macOS.
 * - #VMNL_NET_ECONNRESET: the peer reset the connection, on Windows.
 * - #VMNL_NET_ECONNABORTED: the connection failed, like on a timeout, on Windows.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Closes the sending side only, and the socket still receives. Windows resets a connection shut down for receiving when bytes arrive, unlike POSIX, so no function closes the receiving side.
 * \remark A send after it fails with #VMNL_NET_EPIPE. Closing a TCP socket with received bytes still unread may reset the connection instead of ending it, as RFC 1122 allows (4.2.2.13).
 * \attention \p netsock **MUST** be connected.
 */
bool vmnl_net_socket_shutdown(struct VmnlNetSocket *netsock, VmnlNetError *error);

/**
 * \brief Closes a socket.
 * \param netsock Socket to close.
 * \remark Calls close() on POSIX and closesocket() on Windows, and ignores their errors.
 */
void vmnl_net_socket_close(struct VmnlNetSocket *netsock);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_SOCKET_INTERNAL_H */

/** @} */
