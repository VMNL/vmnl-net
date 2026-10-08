/**
 * \file context_internal.h
 * \brief Internal definition of the context.
 */

#ifndef VMNL_NET_CONTEXT_INTERNAL_H
#define VMNL_NET_CONTEXT_INTERNAL_H

#include <vmnl/net/context.h> /* IWYU pragma: export */

/**
 * \brief State of a context, hidden behind the opaque VmnlNetContext handle.
 */
struct VmnlNetContext {
    VmnlNetAllocator allocator; /*!< Callbacks used to allocate/deallocate memory. */
};

#endif /* !VMNL_NET_CONTEXT_INTERNAL_H */
