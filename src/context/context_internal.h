/**
 * \file context_internal.h
 * \brief Internal definition of the context.
 */

#ifndef VMNL_NET_CONTEXT_INTERNAL_H
#define VMNL_NET_CONTEXT_INTERNAL_H

#include <vmnl/net/context.h> /* IWYU pragma: export */

#include "core/list_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Internal definition of a context.
 */
struct VmnlNetContext {
    VmnlNetAllocator allocator; /*!< Callbacks used to allocate/deallocate memory. */
    VmnlNetList timers; /*!< Running timers, sorted by deadline. */
};

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_CONTEXT_INTERNAL_H */
