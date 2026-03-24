/**
 * @file basic_socket.h
 * @brief Defines basic_socket interface.
 * @author @lszsrd
 * @version 0.1
 * @date March 21, 2026
 */

#ifndef VMNL_NETWORK_SOCKET_BASIC_SOCKET_H_
#define VMNL_NETWORK_SOCKET_BASIC_SOCKET_H_

#include "buffer/buffer.h"

#define __BASIC_SOCKET_INTERNAL                                             \
    + sizeof(long *(*)(const void *)) * 2                                   \
    + sizeof(int) * 2                                                       \
    + sizeof(unsigned short)                                                \
    + sizeof(buffer_t) * 2

/**
 * @struct basic_socket_t
 * @brief Basic socket wrapper.
 *
 * Manages a socket file descriptor and provides basic primitives to handle
 * networking operations.
 */
typedef struct
{
    struct
    {
        const buffer_t *in;                     /*!< Incomming buffer, stores data received from socket. */
        const buffer_t *out;                    /*!< Outgoing buffer, stores data ready to be sent. */
    } buffer;                                   /*!< basic_socket's I/O buffers. */

    char __private[__BASIC_SOCKET_INTERNAL];    /*!< Private data used to manage object. */
} basic_socket_t;

/**
 * @brief Initialize a basic_socket object.
 *
 * On construct, a basic_socket's handle will be in a "non initialized" state and any
 * operation beside of basic_socket_open will fail.
 *
 * @param basic_socket          [in] basic_socket object.
 * @param buffer_constructor    [in] buffer constructor which defines the buffer type to use.
 */
void basic_socket_constructor(basic_socket_t *basic_socket, buffer_constructor_t buffer_constructor);

/**
 * @brief Destruct a basic_socket object.
 *
 * On destruct, all basic_socket's ressources (including buffers) will be freed and
 * handle will be closed. Any further operations on a destructed basic_socket is
 * undefined behavior.
 *
 * @param basic_socket          [in] basic_socket object.
 */
void basic_socket_destructor(const basic_socket_t *basic_socket);

/**
 * Open socket with provided specs.
 *
 * basic_socket's handle will be initialized with a call to socket() with provided domain, type
 * and protocol to use.
 * Invalid parameters may lead to an undefined behavior of the basic_socket object.
 *
 * @param basic_socket          [in] basic_socket object.
 * @param domain                [in] socket's communication domain (i.e. AF_INET, AF_INET6, ...).
 * @param type                  [in] socket's type (i.e. SOCK_STREAM, SOCK_DGRAM, ...).
 * @param protocol              [in] protocol in use.
 *
 * @return true on success, false otherwise.
 */
bool basic_socket_open(const basic_socket_t *basic_socket, int domain, int type, int protocol);

/**
 * Close internal socket handle.
 *
 * If basic socket's handle is in invalid state (i.e. not yet opened or already closed),
 * this function quietly returns and does nothing.
 *
 * @param basic_socket          [in] basic socket object.
 */
void basic_socket_close(const basic_socket_t *basic_socket);

/**
 * Bind basic socket's handle to specified port and start listenning on it.
 *
 * For now, the handle is bound to any address and listens to IPv4 clients.
 *
 * @param basic_socket          [in] basic socket object.
 * @param port                  [in] the port to listen to (requires privilegied
 * process to listen on port bellow 1024).
 *
 * @return true on success, false otherwise.
 */
bool basic_socket_listen(const basic_socket_t *basic_socket, unsigned short port);

/**
 * Connect to a remote host.
 *
 * Do a DNS lookup and connects to the first host:port that matches.
 *
 * @param basic_socket          [in] basic socket object.
 * @param ip                    [in] remote IP address.
 * @param port                  [in] remote port.
 *
 * @return true on success, false otherwise.
 */
bool basic_socket_connect(const basic_socket_t *basic_socket, const char *ip, unsigned short port);

/**
 * Enqueue a message to later send it.
 *
 * Allocates a buffer and pushes it to the internal send buffer.
 *
 * @param basic_socket          [in] basic socket object.
 * @param format                [in] format to use, based on printf.
 * @param ...                   [in] variadic list of arguments.
 *
 * @return true on success, false otherwise.
 */
__attribute__((format(printf, 2, 3)))
bool basic_socket_enqueue(const basic_socket_t *basic_socket, const char *format, ...);

/**
 * Receive a message from connected socket.
 *
 * Calls a function that can be managed by other components. By default, calls a
 * blocking recv().
 *
 * @param basic_socket          [in] basic socket object.
 *
 * @return true on success, false otherwise.
 */
bool basic_socket_recv(const basic_socket_t *basic_socket);

/**
 * Send a message to a socket.
 *
 * If output buffer is empty, does nothing. Data should be pushed via basic_socket_enqueue()
 * or directly managed by buffer's push functions.
 *
 * @param basic_socket          [in] basic socket object.
 *
 * @return true on success, false otherwise.
 */
bool basic_socket_send(const basic_socket_t *basic_socket);

/**
 * Set socket option.
 *
 * Internaly maps to setsockopt() with SOL_SOCKET option name.
 *
 * @param basic_socket          [in] basic socket object.
 * @param option                [in] socket's option.
 *
 * @return true on success, false otherwise.
 */
bool basic_socket_set_option(const basic_socket_t *basic_socket, int option);

#endif /* !VMNL_NETWORK_SOCKET_BASIC_SOCKET_H_ */
