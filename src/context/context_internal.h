/**
 * \file context_internal.h
 * \brief Internal definition of the context.
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
    struct VmnlNetListLink timers; /*!< Running timers, sorted by deadline. */
};

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_INTERNAL_H */
