/**
 * \file context_internal.h
 * \brief Internal definition of the context.
 *
 * On Windows, vmnl_net_context_create() starts Winsock with WSAStartup(), and vmnl_net_context_destroy() stops it with WSACleanup().
 * Winsock counts these calls, so it runs while at least one context lives.
 */

/**
 * \addtogroup Context
 * @{
 */

#ifndef VMNL_NET_CONTEXT_INTERNAL_H
#define VMNL_NET_CONTEXT_INTERNAL_H

#include <vmnl/net/context.h> /* IWYU pragma: export */

#include "list/list_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Context, opaque outside the library.
 */
struct VmnlNetContext {
    struct {
        struct VmnlNetListLink active; /*!< Active timers, sorted by deadline. */
        uint64_t now; /*!< Time the timeouts count from, given by the last vmnl_net_context_advance() or read with vmnl_net_clock_now() at the creation of the context. */
    } timers; /*!< Timers of the context. */
};

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_INTERNAL_H */

/** @} */
