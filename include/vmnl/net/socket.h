/**
 * \file socket.h
 *
 * \defgroup Socket Socket
 * \addtogroup Socket
 * \brief Events shared by the socket families, to wait until a socket can receive or send.
 *
 * Events combine with `|`, like `VMNL_NET_SOCKET_READABLE | VMNL_NET_SOCKET_WRITABLE`, and a wait returns the ones that came.
 * @{
 */

#ifndef VMNL_NET_SOCKET_H
#define VMNL_NET_SOCKET_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Events of a socket, a combination of #VMNL_NET_SOCKET_READABLE and #VMNL_NET_SOCKET_WRITABLE.
 */
typedef uint32_t VmnlNetSocketEvents;

#define VMNL_NET_SOCKET_READABLE UINT32_C(1) /*!< A receive would not fail with #VMNL_NET_EAGAIN. */
#define VMNL_NET_SOCKET_WRITABLE UINT32_C(2) /*!< A send would not fail with #VMNL_NET_EAGAIN. */

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_SOCKET_H */

/** @} */
