/**
 * \file socket_system_internal.h
 * \defgroup SocketSystem System socket
 * \addtogroup SocketSystem
 * \brief Internal wrapper over the BSD sockets and Winsock2.
 *
 * The wrapper merges the BSD sockets and Winsock2 into one set of functions, which each kind with a system socket reuses instead of calling the systems itself.
 * A socket is always non-blocking and never inherited by child processes. An IPv6 socket also reaches IPv4 peers.
 * A system failure is translated into a code of the library through vmnl_net_error_from_last_system().
 * @{
 */

#ifndef VMNL_NET_SOCKET_SYSTEM_INTERNAL_H
#define VMNL_NET_SOCKET_SYSTEM_INTERNAL_H

#include <vmnl/net/address.h>
#include <vmnl/net/buffer.h>
#include <vmnl/net/clock.h>
#include <vmnl/net/handle.h>
#include <vmnl/net/socket.h>

#include "address/address_internal.h"
#include "error/error_internal.h"

#include <limits.h>

/**
 * \def VMNL_NET_SOCKET_SYSTEM_INVALID
 * \brief Handle that socket() and accept() return on failure.
 * \remark INVALID_SOCKET on Windows, -1 elsewhere.
 */
/**
 * \def VMNL_NET_SOCKET_SYSTEM_SHUTDOWN_SEND
 * \brief Direction that vmnl_net_socket_system_shutdown() closes.
 * \remark SD_SEND on Windows, SHUT_WR elsewhere.
 */
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#define VMNL_NET_SOCKET_SYSTEM_INVALID       INVALID_SOCKET
#define VMNL_NET_SOCKET_SYSTEM_SHUTDOWN_SEND SD_SEND

_Static_assert(sizeof(VmnlNetBufferView) == sizeof(WSABUF), "a VmnlNetBufferView must have the layout of a WSABUF");
_Static_assert(offsetof(VmnlNetBufferView, size) == offsetof(WSABUF, len), "a VmnlNetBufferView must have the layout of a WSABUF");
_Static_assert(offsetof(VmnlNetBufferView, data) == offsetof(WSABUF, buf), "a VmnlNetBufferView must have the layout of a WSABUF");
#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <sys/socket.h>

#define VMNL_NET_SOCKET_SYSTEM_INVALID       (-1)
#define VMNL_NET_SOCKET_SYSTEM_SHUTDOWN_SEND SHUT_WR

_Static_assert(sizeof(VmnlNetBufferView) == sizeof(struct iovec), "a VmnlNetBufferView must have the layout of a struct iovec");
_Static_assert(offsetof(VmnlNetBufferView, data) == offsetof(struct iovec, iov_base), "a VmnlNetBufferView must have the layout of a struct iovec");
_Static_assert(offsetof(VmnlNetBufferView, size) == offsetof(struct iovec, iov_len), "a VmnlNetBufferView must have the layout of a struct iovec");
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED UINT32_C(1) /*!< vmnl_net_socket_system_connect() succeeded, so that vmnl_net_socket_system_send_to() refuses an address on every system. */
#define VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST UINT32_C(2) /*!< vmnl_net_socket_system_set_broadcast() allowed the broadcast, which vmnl_net_socket_system_check_remote() reads without a system call. */
#define VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED   UINT32_C(4) /*!< A send or a connect of a datagram socket drew ECONNREFUSED on POSIX, which the next receive reports, as Windows only reports it at the receive. */

/**
 * \brief System socket, its address family, its type and the state the library reads back.
 */
struct VmnlNetSocketSystem {
    VmnlNetHandle handle; /*!< System socket, a SOCKET on Windows and a file descriptor elsewhere. */
    int family; /*!< AF_INET or AF_INET6, used to convert the addresses passed to the socket. */
    int type; /*!< SOCK_STREAM or SOCK_DGRAM, used to handle the ICMP errors of a datagram socket on Windows. */
    uint32_t flags; /*!< #VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED, #VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST and #VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED, 0 at the open. */
};

#ifdef _WIN32
/**
 * \brief Sets whether a datagram socket reports an ICMP error that an earlier datagram drew, on Windows.
 * \param netsock Datagram socket to set.
 * \param control SIO_UDP_CONNRESET for a port unreachable, reported with WSAECONNRESET, or SIO_UDP_NETRESET for an expired time to live, reported with WSAENETRESET.
 * \param enabled true to report the error, false to drop it.
 * \return true on success, false otherwise, with the error in WSAGetLastError().
 * \remark Windows reports both errors on any datagram socket by default, while POSIX reports a port unreachable on a connected socket only.
 */
static inline bool vmnl_net_socket_system_report_unreachable(const struct VmnlNetSocketSystem *netsock, DWORD control, BOOL enabled)
{
    return WSAIoctl(netsock->handle, control, &enabled, sizeof enabled, NULL, 0, &(DWORD) {0}, NULL, NULL) == 0;
}
#endif

/**
 * \brief Sets an integer option of a socket.
 * \param netsock Socket to set.
 * \param level Level of the option, like SOL_SOCKET or IPPROTO_TCP.
 * \param name Option to set, like SO_KEEPALIVE.
 * \param value Value of the option.
 * \param error Error code of the call, nullable, a translated system error on failure.
 * \return true on success, false otherwise.
 * \remark Passes \p value through a const char pointer, which Windows requires and POSIX accepts.
 * \attention \p name **MUST** be an option that behaves the same on Windows, macOS and Linux, as the wrapper hides the differences between systems.
 */
static inline bool vmnl_net_socket_system_set_option(struct VmnlNetSocketSystem *netsock, int level, int name, int value, VmnlNetError *error)
{
    /* GCOVR_EXCL_START: on Linux, where the coverage runs, the options of the wrapper only fail on a socket that is not open. */
    if (setsockopt(netsock->handle, level, name, (const char *) &value, sizeof value) == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    /* GCOVR_EXCL_STOP */
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}

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
 * - A translated error of vmnl_net_socket_system_non_blocking(), of vmnl_net_socket_system_no_inheritance() on POSIX, or an error of vmnl_net_socket_system_dual_stack().
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Calls vmnl_net_socket_system_dual_stack() on an AF_INET6 socket.
 * \remark Creates the socket not inherited with WSA_FLAG_NO_HANDLE_INHERIT on Windows, and calls vmnl_net_socket_system_no_inheritance() on POSIX.
 * \remark Clears SIO_UDP_CONNRESET and SIO_UDP_NETRESET with vmnl_net_socket_system_report_unreachable() on a datagram socket, on Windows.
 * \remark Closes the socket if one of its settings fails.
 * \attention A context **MUST** exist while the socket is open, as the contexts start Winsock on Windows.
 */
bool vmnl_net_socket_system_open(int family, int type, struct VmnlNetSocketSystem *netsock, VmnlNetError *error);

/**
 * \brief Makes a socket non-blocking.
 *
 * A call that would wait then fails with #VMNL_NET_EAGAIN.
 * \param netsock Socket to set.
 * \return true on success, false otherwise, with the error in errno, or WSAGetLastError() on Windows.
 * \remark Sets O_NONBLOCK with fcntl() on POSIX, and FIONBIO with ioctlsocket() on Windows.
 * \attention \p netsock **MUST** be a socket just opened or accepted, as POSIX replaces its other status flags, which such a socket does not have.
 */
bool vmnl_net_socket_system_non_blocking(struct VmnlNetSocketSystem *netsock);

/**
 * \brief Keeps a socket out of the child processes.
 * \param netsock Socket to set.
 * \return true on success, false otherwise, with the error in errno, or GetLastError() on Windows.
 * \remark Sets FD_CLOEXEC with fcntl() on POSIX, and clears HANDLE_FLAG_INHERIT with SetHandleInformation() on Windows.
 */
bool vmnl_net_socket_system_no_inheritance(struct VmnlNetSocketSystem *netsock);

/**
 * \brief Lets a listener rebind a port still in TIME_WAIT.
 * \param netsock Socket to set.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_REUSEADDR on POSIX. Windows gets SO_EXCLUSIVEADDRUSE instead, as its SO_REUSEADDR lets another socket take a port in use.
 * \attention It **MUST** be called before vmnl_net_socket_system_bind().
 */
bool vmnl_net_socket_system_reuse_address(struct VmnlNetSocketSystem *netsock, VmnlNetError *error);

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
static inline bool vmnl_net_socket_system_dual_stack(struct VmnlNetSocketSystem *netsock, VmnlNetError *error)
{
    return vmnl_net_socket_system_set_option(netsock, IPPROTO_IPV6, IPV6_V6ONLY, 0, error);
}

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
static inline bool vmnl_net_socket_system_set_no_delay(struct VmnlNetSocketSystem *netsock, bool enabled, VmnlNetError *error)
{
    return vmnl_net_socket_system_set_option(netsock, IPPROTO_TCP, TCP_NODELAY, (int) enabled, error);
}

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
static inline bool vmnl_net_socket_system_set_keep_alive(struct VmnlNetSocketSystem *netsock, bool enabled, VmnlNetError *error)
{
    return vmnl_net_socket_system_set_option(netsock, SOL_SOCKET, SO_KEEPALIVE, (int) enabled, error);
}

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
bool vmnl_net_socket_system_set_linger(struct VmnlNetSocketSystem *netsock, bool enabled, uint16_t seconds, VmnlNetError *error);

/**
 * \brief Sets the size of the system's receive buffer.
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p size is 0 or greater than INT_MAX, or the peer reset the connection on macOS.
 * - #VMNL_NET_ENETRESET: the connection timed out with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOTCONN: the connection was reset with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_RCVBUF. Linux doubles the size, for its own bookkeeping.
 * \remark Refuses a \p size of 0 itself, as only macOS refuses it, while Linux takes its minimum and Windows no buffer.
 */
bool vmnl_net_socket_system_set_receive_buffer_size(struct VmnlNetSocketSystem *netsock, size_t size, VmnlNetError *error);

/**
 * \brief Sets the size of the system's send buffer.
 * \param netsock Socket to set.
 * \param size Size of the buffer, in bytes.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p size is 0 or greater than INT_MAX, or the peer reset the connection on macOS.
 * - #VMNL_NET_ENETRESET: the connection timed out with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOTCONN: the connection was reset with keep-alive enabled, on Windows.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_SNDBUF. Linux doubles the size, for its own bookkeeping.
 * \remark Refuses a \p size of 0 itself, as only macOS refuses it, while Linux takes its minimum and Windows no buffer.
 */
bool vmnl_net_socket_system_set_send_buffer_size(struct VmnlNetSocketSystem *netsock, size_t size, VmnlNetError *error);

/**
 * \brief Lets a datagram socket send to a broadcast address, like 255.255.255.255.
 * \param netsock Socket to set.
 * \param enabled true to allow the broadcast, false to forbid it.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Sets SO_BROADCAST, and adds or removes #VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST. An IPv6 socket broadcasts to IPv4 peers through ::ffff:255.255.255.255, as IPv6 itself has no broadcast.
 */
bool vmnl_net_socket_system_set_broadcast(struct VmnlNetSocketSystem *netsock, bool enabled, VmnlNetError *error);

/**
 * \brief Reads and clears the pending error of a socket.
 * \param netsock Socket to read.
 * \param error Error code of the call, nullable:
 * - The translated pending code, like an error of vmnl_net_socket_system_connect() after a failed connection.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on macOS.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true if no error is pending, false otherwise.
 * \remark Reads SO_ERROR.
 */
bool vmnl_net_socket_system_pending_error(struct VmnlNetSocketSystem *netsock, VmnlNetError *error);

/**
 * \brief Gets the local address of a socket.
 * \param netsock Socket to read.
 * \param address Local address, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOBUFS: the system lacks resources, on POSIX.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Calls getsockname(), which gives the port the system chose after a bind to #VMNL_NET_ADDRESS_ANY_PORT.
 * \remark An unbound socket gives the wildcard address of its family with port 0, which Windows reports with WSAEINVAL.
 */
bool vmnl_net_socket_system_local_address(const struct VmnlNetSocketSystem *netsock, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Converts events into the events of poll() and WSAPoll().
 * \param socket_events Events to convert, known ones only.
 * \return POLLIN for #VMNL_NET_SOCKET_READABLE, ORed with POLLOUT for #VMNL_NET_SOCKET_WRITABLE.
 * \remark Never asks POLLPRI, which makes WSAPoll() fail.
 */
static inline short vmnl_net_socket_system_poll_events(VmnlNetSocketEvents socket_events)
{
    unsigned int events = 0;

    if ((socket_events & VMNL_NET_SOCKET_READABLE) != 0) {
        events |= POLLIN;
    }
    if ((socket_events & VMNL_NET_SOCKET_WRITABLE) != 0) {
        events |= POLLOUT;
    }
    return (short) events;
}

/**
 * \brief Converts the events that poll() or WSAPoll() returned into events.
 * \param events Events asked.
 * \param revents Events returned for the socket.
 * \return Events of \p events that came, every one of \p events when POLLERR, POLLHUP or POLLNVAL tells that the next call reports an error.
 */
static inline VmnlNetSocketEvents vmnl_net_socket_system_ready_events(VmnlNetSocketEvents events, short revents)
{
    unsigned int returned = (unsigned int) revents;

    if ((returned & POLLERR) != 0 || (returned & POLLHUP) != 0 || (returned & POLLNVAL) != 0) { /* GCOVR_EXCL_BR_LINE: a datagram socket never gets POLLHUP, and POLLNVAL needs a socket that is not open. */
        return events;
    }
    return events & ((((returned & POLLIN) != 0) ? VMNL_NET_SOCKET_READABLE : 0) | (((returned & POLLOUT) != 0) ? VMNL_NET_SOCKET_WRITABLE : 0));
}

/**
 * \brief Converts the time left of a wait into the timeout of poll() and WSAPoll().
 * \param timeout Nanoseconds of the whole wait, #VMNL_NET_CLOCK_INF without limit.
 * \param elapsed Nanoseconds already waited.
 * \return Milliseconds left, rounded up so the wait never ends early and capped at INT_MAX, -1 without limit.
 */
static inline int vmnl_net_socket_system_poll_timeout(uint64_t timeout, uint64_t elapsed)
{
    uint64_t per_millisecond = VMNL_NET_NANOSECONDS_PER_SECOND / VMNL_NET_MILLISECONDS_PER_SECOND;
    uint64_t left = (elapsed >= timeout) ? 0 : timeout - elapsed;
    uint64_t milliseconds = (left / per_millisecond) + (((left % per_millisecond) != 0) ? 1 : 0);

    if (timeout == VMNL_NET_CLOCK_INF) {
        return -1;
    }
    return (milliseconds > INT_MAX) ? INT_MAX : (int) milliseconds;
}

/**
 * \brief Waits until a socket can receive or send.
 *
 * Calls poll() on POSIX and WSAPoll() on Windows, again with the time left while the time is not over, so a capped timeout or a signal does not end the wait early.
 * POSIX interrupts poll() with EINTR when a handled signal comes, and Windows has no such case, so the wait never reports it.
 * A socket with #VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED is ready for every event asked at once, as its next receive reports an error.
 * \param netsock Socket to wait for.
 * \param events Events to wait for, known ones only.
 * \param timeout Nanoseconds to wait at most, 0 to check without waiting, #VMNL_NET_CLOCK_INF to wait without limit.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ETIMEDOUT: none of \p events came within \p timeout.
 * - #VMNL_NET_ENOMEM or #VMNL_NET_ENOBUFS: the system lacks memory.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return Events of \p events that came, see vmnl_net_socket_system_ready_events(), 0 on failure.
 */
VmnlNetSocketEvents vmnl_net_socket_system_wait_for(const struct VmnlNetSocketSystem *netsock, VmnlNetSocketEvents events, uint64_t timeout, VmnlNetError *error);

/**
 * \brief Binds a socket to a local address.
 * \param netsock Socket to bind.
 * \param address Local address, with #VMNL_NET_ADDRESS_ANY_PORT to let the system choose the port.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAFNOSUPPORT: \p address is an IPv6 address and \p netsock an AF_INET socket.
 * - #VMNL_NET_EADDRINUSE: another socket holds \p address, or no ephemeral port is left for #VMNL_NET_ADDRESS_ANY_PORT on Linux.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address does not belong to the host.
 * - #VMNL_NET_EACCES: the port needs privileges, like a port below 1024 on Linux, or \p address is a broadcast address without vmnl_net_socket_system_set_broadcast() on Windows.
 * - #VMNL_NET_EINVAL: \p netsock is already bound, or a link-local \p address has scope 0 on Linux or a scope naming no interface on macOS.
 * - #VMNL_NET_FSYSTEM: a link-local \p address has a scope naming no interface, on Linux.
 * - #VMNL_NET_ENOBUFS: the system lacks buffers, on Windows.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Converts \p address for the family of \p netsock, an IPv4 address staying mapped on an AF_INET6 socket.
 */
bool vmnl_net_socket_system_bind(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, VmnlNetError *error);

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
bool vmnl_net_socket_system_listen(struct VmnlNetSocketSystem *netsock, VmnlNetError *error);

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
 * - A translated error of vmnl_net_socket_system_non_blocking() or vmnl_net_socket_system_no_inheritance().
 * - Any other translated system error.
 * \return true on success, false otherwise.
 * \remark Makes the accepted socket non-blocking and not inherited, and closes it if one of them fails. Linux does not copy O_NONBLOCK from the listening socket.
 * \remark The accepted socket gets the peer's family, AF_INET6 for an IPv4 peer of an AF_INET6 listener.
 * \remark The accepted socket starts without flags, as each flag belongs to one connection, whatever the listening socket holds.
 */
bool vmnl_net_socket_system_accept(struct VmnlNetSocketSystem *netsock, struct VmnlNetSocketSystem *peer, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Refuses a remote address that the systems treat differently.
 *
 * A wildcard address, like 0.0.0.0, connects to the host on Linux and macOS, while Windows refuses it, and a send to it reaches the host on Linux only.
 * Port 0 fails on macOS, while Linux connects to it and Windows sends to it.
 * Linux refuses the limited broadcast address without SO_BROADCAST, while macOS and Windows accept the connection, and Windows then sends to it.
 * An IPv6 socket of Windows also sends to the mapped ::ffff:255.255.255.255 without it, where macOS and Linux refuse the send.
 * Reads #VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST when \p address is 255.255.255.255, mapped or not, so a SO_BROADCAST set on the system socket outside the library is not seen.
 * Leaves a directed broadcast address, like 192.168.1.255, to the system.
 * \param netsock Socket to connect or to send on.
 * \param address Remote address.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EADDRNOTAVAIL: \p address is a wildcard, or has port 0.
 * - #VMNL_NET_EACCES: \p netsock is a datagram socket without #VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST, and \p address is 255.255.255.255.
 * \return true if \p netsock may reach \p address, false otherwise.
 */
static inline bool vmnl_net_socket_system_check_remote(const struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, VmnlNetError *error)
{
    if (vmnl_net_address_is_any(address) == true || address->port == VMNL_NET_ADDRESS_ANY_PORT) {
        vmnl_net_error_set(error, VMNL_NET_EADDRNOTAVAIL);
        return false;
    }
    if (netsock->type == SOCK_DGRAM && (netsock->flags & VMNL_NET_SOCKET_SYSTEM_FLAG_BROADCAST) == 0 && vmnl_net_address_is_broadcast(address) == true) {
        vmnl_net_error_set(error, VMNL_NET_EACCES);
        return false;
    }
    return true;
}

/**
 * \brief Refuses views that the systems treat differently.
 *
 * macOS refuses more than INT_MAX bytes, Linux and macOS more than IOV_MAX views, while Windows takes both.
 * \param views Views to send or to fill.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nviews is 0, or \p views hold more than INT_MAX bytes.
 * - #VMNL_NET_EMSGSIZE: \p nviews is more than #VMNL_NET_SOCKET_MAX_VIEWS.
 * \return true if a system call may take \p views, false otherwise.
 */
static inline bool vmnl_net_socket_system_check_views(const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    size_t size = 0;

    if (nviews == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (nviews > VMNL_NET_SOCKET_MAX_VIEWS) {
        vmnl_net_error_set(error, VMNL_NET_EMSGSIZE);
        return false;
    }
    for (size_t it = 0; it < nviews; it++) {
        if (views[it].size > (size_t) INT_MAX - size) {
            vmnl_net_error_set(error, VMNL_NET_EINVAL);
            return false;
        }
        size += views[it].size;
    }
    return true;
}

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
 * - #VMNL_NET_EACCES: \p address is 255.255.255.255 on a datagram socket without vmnl_net_socket_system_set_broadcast(), or a directed broadcast address or a firewall or security rule forbids the connection on Linux.
 * - #VMNL_NET_EPERM: the same cases as #VMNL_NET_EACCES, on Linux.
 * - #VMNL_NET_EADDRINUSE: the local address is already in use.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address is a wildcard like 0.0.0.0 or has port 0, or no ephemeral port is left on Linux.
 * - #VMNL_NET_EAGAIN: the routing cache lacks entries, on Linux.
 * - #VMNL_NET_EINVAL: \p netsock is listening, or a previous connection has not completed yet, on Windows.
 * - #VMNL_NET_ENOTSUP: \p netsock is listening, on macOS.
 * - #VMNL_NET_ENOBUFS: the system lacks buffers, on macOS and Windows.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - Any other translated system error.
 * \return true if the connection completed at once, false otherwise.
 * \remark Windows reports a connection in progress with WSAEWOULDBLOCK. It gives #VMNL_NET_EINPROGRESS here, where vmnl_net_error_from_system() gives #VMNL_NET_EAGAIN.
 * \remark A stream socket becomes writable once the connection completes, and vmnl_net_socket_system_pending_error() then gives its result.
 * \remark Calls vmnl_net_socket_system_check_remote() first, and adds #VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED on success.
 * \remark A refusal pending on a datagram socket, which macOS reports at the connect, goes to #VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED and the connect is done again, so the next receive reports it on every system, as Linux and Windows keep it across a connect.
 * \remark Sets SIO_UDP_CONNRESET again with vmnl_net_socket_system_report_unreachable() once a datagram socket is connected, on Windows.
 */
bool vmnl_net_socket_system_connect(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, VmnlNetError *error);

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
 * - #VMNL_NET_EMSGSIZE: the datagram is too large for the socket, or \p nviews is more than #VMNL_NET_SOCKET_MAX_VIEWS.
 * - #VMNL_NET_EAGAIN: the system's send buffer is full, or no ephemeral port is left for an unbound socket on Linux.
 * - #VMNL_NET_EACCES: \p address is 255.255.255.255 without vmnl_net_socket_system_set_broadcast(), or a directed broadcast address without it or a prohibit route reaches it on Linux.
 * - #VMNL_NET_ENETUNREACH or #VMNL_NET_EHOSTUNREACH: no route reaches \p address.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - #VMNL_NET_ENETRESET: the time to live expired, on Windows.
 * - #VMNL_NET_EADDRNOTAVAIL: \p address is a wildcard like 0.0.0.0 or has port 0, or the local address is no longer available on macOS.
 * - #VMNL_NET_EISCONN: \p address is not NULL and \p netsock has #VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED, as only macOS refuses it.
 * - #VMNL_NET_ENOTCONN: \p address is NULL and \p netsock is not connected.
 * - #VMNL_NET_EPIPE: vmnl_net_socket_system_shutdown() stopped sending on \p netsock.
 * - #VMNL_NET_EINVAL: \p nviews is 0, \p views hold more than INT_MAX bytes, or a blackhole route reaches \p address on Linux.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory.
 * - Any other translated system error.
 * \return Number of bytes sent, the whole datagram on a datagram socket, 0 with #VMNL_NET_SUCCESS for an empty datagram, 0 on failure.
 * \remark With a NULL \p address on a stream socket, it behaves like vmnl_net_socket_system_send(), whose errors apply.
 * \remark Calls vmnl_net_socket_system_check_views(), then vmnl_net_socket_system_check_remote() when \p address is not NULL.
 * \remark A datagram that draws ECONNREFUSED on POSIX is sent again, and #VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED keeps the refusal for the next receive, as Windows only reports it at the receive.
 */
size_t vmnl_net_socket_system_send_to(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error);

/**
 * \brief Receives one datagram and the address of its sender.
 * \param netsock Socket to receive from, a datagram socket unless \p address is NULL.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param address Address of the sender, NULL to skip it, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: no datagram is waiting, an unbound socket included.
 * - #VMNL_NET_EMSGSIZE: the datagram was larger than \p views, and its end is lost, or \p nviews is more than #VMNL_NET_SOCKET_MAX_VIEWS.
 * - #VMNL_NET_ECONNREFUSED: an earlier datagram found no receiver, on a connected socket.
 * - #VMNL_NET_ENETDOWN: the network subsystem failed, on Windows.
 * - #VMNL_NET_EINVAL: \p nviews is 0, or \p views hold more than INT_MAX bytes.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory, on POSIX.
 * - Any other translated system error.
 * \return Number of bytes of the datagram, 0 with #VMNL_NET_SUCCESS for an empty datagram, 0 on failure.
 * \remark With a NULL \p address on a stream socket, it behaves like vmnl_net_socket_system_receive(), whose errors apply.
 * \remark Calls vmnl_net_socket_system_check_views(), then reports #VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED, as #VMNL_NET_ECONNREFUSED, and removes it.
 * \remark Windows refuses a receive on an unbound datagram socket with WSAEINVAL, which gives #VMNL_NET_EAGAIN here, like POSIX.
 */
size_t vmnl_net_socket_system_receive_from(struct VmnlNetSocketSystem *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Sends bytes on a connected socket.
 * \param netsock Socket to send on.
 * \param views Bytes to send, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EAGAIN: the system's send buffer is full.
 * - #VMNL_NET_EPIPE: vmnl_net_socket_system_shutdown() stopped sending, or the connection is closed on POSIX, a reset included on macOS.
 * - #VMNL_NET_ECONNRESET: the peer reset the connection.
 * - #VMNL_NET_ETIMEDOUT: the peer stopped acknowledging the data, on Linux.
 * - #VMNL_NET_ECONNABORTED: the connection failed, like on a timeout, on Windows.
 * - #VMNL_NET_ENETRESET: keep-alive detected a failure, on Windows.
 * - #VMNL_NET_ENOTCONN: \p netsock is not connected.
 * - #VMNL_NET_ENETUNREACH or #VMNL_NET_EHOSTUNREACH: no route reaches the peer, on macOS.
 * - #VMNL_NET_ENETDOWN: the local interface is down on macOS, or the network subsystem failed on Windows.
 * - #VMNL_NET_EADDRNOTAVAIL: the local address is no longer available, on macOS.
 * - #VMNL_NET_ENOBUFS or #VMNL_NET_ENOMEM: the system lacks memory.
 * - #VMNL_NET_EMSGSIZE: \p nviews is more than #VMNL_NET_SOCKET_MAX_VIEWS.
 * - #VMNL_NET_EINVAL: \p nviews is 0, or \p views hold more than INT_MAX bytes.
 * - Any other translated system error.
 * \return Number of bytes sent, which can be less than \p views holds, 0 on failure.
 * \remark Never raises SIGPIPE.
 */
static inline size_t vmnl_net_socket_system_send(struct VmnlNetSocketSystem *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return vmnl_net_socket_system_send_to(netsock, NULL, views, nviews, error);
}

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
 * - #VMNL_NET_EMSGSIZE: \p nviews is more than #VMNL_NET_SOCKET_MAX_VIEWS.
 * - #VMNL_NET_EINVAL: \p nviews is 0, or \p views hold more than INT_MAX bytes.
 * - Any other translated system error.
 * \return Number of bytes received, 0 with #VMNL_NET_SUCCESS when the peer closed the connection, 0 on failure.
 */
static inline size_t vmnl_net_socket_system_receive(struct VmnlNetSocketSystem *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return vmnl_net_socket_system_receive_from(netsock, views, nviews, NULL, error);
}

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
bool vmnl_net_socket_system_shutdown(struct VmnlNetSocketSystem *netsock, VmnlNetError *error);

/**
 * \brief Closes a socket.
 * \param netsock Socket to close.
 * \remark Calls close() on POSIX and closesocket() on Windows, and ignores their errors.
 */
void vmnl_net_socket_system_close(struct VmnlNetSocketSystem *netsock);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_SOCKET_SYSTEM_INTERNAL_H */

/** @} */
