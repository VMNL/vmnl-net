/**
 * \file handle.h
 *
 * \defgroup Handle Handle
 * \addtogroup Handle
 * \brief Native handle of a socket, to pass it to the system's own functions.
 * @{
 */

#ifndef VMNL_NET_HANDLE_H
#define VMNL_NET_HANDLE_H

/**
 * \typedef VmnlNetHandle
 * \brief System socket, a SOCKET on Windows and a file descriptor elsewhere.
 */
#ifdef _WIN32
#include <stdint.h>

typedef uintptr_t VmnlNetHandle;
#else
typedef int VmnlNetHandle;
#endif

#endif /* !VMNL_NET_HANDLE_H */

/** @} */
