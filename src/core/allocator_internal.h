/**
 * \file allocator_internal.h
 * \brief Functions used by the library to allocate memory, set by vmnl_net_set_allocator().
 */

#ifndef VMNL_NET_ALLOCATOR_INTERNAL_H
#define VMNL_NET_ALLOCATOR_INTERNAL_H

#include <vmnl/net/core.h>

typedef struct VmnlNetAllocator {
    VmnlNetAllocate allocate; /*!< Allocates memory. */
    VmnlNetReallocate reallocate; /*!< Reallocates memory. */
    VmnlNetDeallocate deallocate; /*!< Releases memory. */
} VmnlNetAllocator;

extern VmnlNetAllocator vmnl_net_allocator;

#endif /* !VMNL_NET_ALLOCATOR_INTERNAL_H */
