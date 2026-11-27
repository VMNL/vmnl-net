/**
 * \file socket_datagram_internal.h
 * \brief Internal definition of the datagram sockets.
 *
 * Every type of datagram socket starts with a VmnlNetSocketDatagram that points to its VmnlNetSocketDatagramOps. A public function checks its arguments, then calls the operation of the type.
 */

/**
 * \addtogroup SocketDatagram
 * @{
 */

#ifndef VMNL_NET_SOCKET_DATAGRAM_INTERNAL_H
#define VMNL_NET_SOCKET_DATAGRAM_INTERNAL_H

#include <vmnl/net/socket/datagram.h> /* IWYU pragma: export */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Operations of one type of datagram socket.
 * \attention An operation **MUST** be called once its public function checked the arguments, as it does not check them again.
 */
struct VmnlNetSocketDatagramOps {
    VmnlNetSocketDatagram *(*create)(const struct VmnlNetSocketDatagramOps *ops, VmnlNetAddressVersion version, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_create(), receiving this table as ops. */
    bool (*bind)(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_bind(). */
    bool (*connect)(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_connect(). */
    bool (*set_broadcast)(VmnlNetSocketDatagram *netsock, bool enabled, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_set_broadcast(), NULL if the type has no broadcast. */
    bool (*set_receive_buffer_size)(VmnlNetSocketDatagram *netsock, size_t size, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_set_receive_buffer_size(), NULL if the type has no system buffer. */
    bool (*set_send_buffer_size)(VmnlNetSocketDatagram *netsock, size_t size, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_set_send_buffer_size(), NULL if the type has no system buffer. */
    bool (*local_address)(const VmnlNetSocketDatagram *netsock, VmnlNetAddress *address, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_local_address(). */
    VmnlNetHandle (*handle)(const VmnlNetSocketDatagram *netsock); /*!< Implements vmnl_net_socket_datagram_handle(). */
    VmnlNetSocketEvents (*wait_for)(const VmnlNetSocketDatagram *netsock, VmnlNetSocketEvents events, uint64_t timeout, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_wait_for(), with known events only. */
    size_t (*send_to)(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_send_to(), and vmnl_net_socket_datagram_send() with a NULL address. */
    size_t (*receive_from)(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error); /*!< Implements vmnl_net_socket_datagram_receive_from(), and vmnl_net_socket_datagram_receive() with a NULL address. */
    void (*destroy)(VmnlNetSocketDatagram *netsock); /*!< Implements vmnl_net_socket_datagram_destroy(). */
};

/**
 * \brief Common part of every type of datagram socket, opaque outside the library.
 */
struct VmnlNetSocketDatagram {
    const struct VmnlNetSocketDatagramOps *ops; /*!< Operations of the type. */
};

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_SOCKET_DATAGRAM_INTERNAL_H */

/** @} */
