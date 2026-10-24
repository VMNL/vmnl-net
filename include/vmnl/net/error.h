/**
 * \file error.h
 *
 * \defgroup Error Error
 * \addtogroup Error
 * \brief Error codes of the library.
 *
 * A function that can fail gives its failure value in its documented return, and writes its code to a nullable #VmnlNetError pointer, its last parameter.
 *
 * \attention A pointer parameter **MUST** not be NULL unless documented as nullable: like the libc, the library uses it without checking, so a NULL one is a bug of the caller, not an error code.
 * \remark A destroy function accepts NULL and does nothing, like free().
 *
 * \par Codes
 * - The code is written on every call, #VMNL_NET_SUCCESS included: it only describes the last call that received it.
 * - E codes are named after errno, F codes are specific to the library.
 * - A code keeps its value on every platform and in every version, and the value of a removed code is never reused.
 *
 * \par Example
 * \code
 * VmnlNetError error;
 *
 * if (vmnl_net_buffer_consume(buffer, 64, &error) == false) { // buffer holds less than 64 bytes
 *     printf("%s\n", vmnl_net_error_as_string(error));        // Invalid argument
 * }
 * vmnl_net_buffer_consume(buffer, 64, NULL);                  // fails too, without writing a code
 * \endcode
 * @{
 */

#ifndef VMNL_NET_ERROR_H
#define VMNL_NET_ERROR_H

#include <vmnl/net_api.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Error code written by the functions that can fail.
 */
typedef uint32_t VmnlNetError;

#define VMNL_NET_SUCCESS       UINT32_C(0) /*!< Call succeeded. */
#define VMNL_NET_EINVAL        UINT32_C(1) /*!< Invalid argument. */
#define VMNL_NET_ENOMEM        UINT32_C(2) /*!< Cannot allocate memory. */
#define VMNL_NET_ENOBUFS       UINT32_C(3) /*!< No buffer space available. */
#define VMNL_NET_EAGAIN        UINT32_C(4) /*!< Operation would block, retry later. */
#define VMNL_NET_EINTR         UINT32_C(5) /*!< Interrupted by a signal. */
#define VMNL_NET_ENOTSUP       UINT32_C(6) /*!< Operation not supported. */
#define VMNL_NET_EACCES        UINT32_C(7) /*!< Permission denied, e.g. binding a privileged port. */
#define VMNL_NET_EPERM         UINT32_C(8) /*!< Operation not permitted, e.g. blocked by a firewall. */
#define VMNL_NET_EMFILE        UINT32_C(9) /*!< Too many open files in the process. */
#define VMNL_NET_ENFILE        UINT32_C(10) /*!< Too many open files in the system. */
#define VMNL_NET_EAFNOSUPPORT  UINT32_C(11) /*!< Address family not supported, e.g. IPv6 disabled. */
#define VMNL_NET_EADDRINUSE    UINT32_C(12) /*!< Address already in use. */
#define VMNL_NET_EADDRNOTAVAIL UINT32_C(13) /*!< Address not available on this host. */
#define VMNL_NET_ENETDOWN      UINT32_C(14) /*!< Network is down. */
#define VMNL_NET_ENETUNREACH   UINT32_C(15) /*!< Network is unreachable. */
#define VMNL_NET_ENETRESET     UINT32_C(16) /*!< Connection dropped by a network reset. */
#define VMNL_NET_EHOSTUNREACH  UINT32_C(17) /*!< No route to host. */
#define VMNL_NET_EINPROGRESS   UINT32_C(18) /*!< Non-blocking connection in progress. */
#define VMNL_NET_EALREADY      UINT32_C(19) /*!< Connection already in progress. */
#define VMNL_NET_EISCONN       UINT32_C(20) /*!< Socket already connected. */
#define VMNL_NET_ENOTCONN      UINT32_C(21) /*!< Socket not connected. */
#define VMNL_NET_ECONNREFUSED  UINT32_C(22) /*!< Connection refused by the peer. */
#define VMNL_NET_ECONNRESET    UINT32_C(23) /*!< Connection reset by the peer. */
#define VMNL_NET_ECONNABORTED  UINT32_C(24) /*!< Connection aborted by this host. */
#define VMNL_NET_ETIMEDOUT     UINT32_C(25) /*!< Connection timed out. */
#define VMNL_NET_EPIPE         UINT32_C(26) /*!< Writing to a closed connection. */
#define VMNL_NET_EMSGSIZE      UINT32_C(27) /*!< Message too large for the socket, e.g. a datagram. */
#define VMNL_NET_EAI_NONAME    UINT32_C(28) /*!< Unknown host or service. */
#define VMNL_NET_EAI_AGAIN     UINT32_C(29) /*!< Temporary failure in name resolution, retry later. */
#define VMNL_NET_EAI_FAIL      UINT32_C(30) /*!< Permanent failure in name resolution. */
#define VMNL_NET_FSYSTEM       UINT32_C(42) /*!< An OS call failed with an error that has no dedicated code (errno, GetLastError() or WSAGetLastError()). */

/**
 * \brief Gets the message of an error code.
 * \param error Error code.
 * \return Static string, cannot be NULL, "Unknown error" for a code the library does not define.
 * \remark O(1), no allocation, thread-safe.
 */
VMNL_NET_API const char *vmnl_net_error_as_string(const VmnlNetError error);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ERROR_H */

/** @} */
