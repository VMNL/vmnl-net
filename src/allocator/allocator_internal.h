/**
 * \file allocator_internal.h
 * \brief Internal definition of the allocator.
 */

/**
 * \addtogroup Allocator
 * @{
 */

#ifndef VMNL_NET_ALLOCATOR_INTERNAL_H
#define VMNL_NET_ALLOCATOR_INTERNAL_H

#include <vmnl/net/allocator.h> /* IWYU pragma: export */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Memory allocator object.
 */
typedef struct {
    VmnlNetAllocatorAllocate allocate; /*!< Equivalent of malloc(). */
    VmnlNetAllocatorReallocate reallocate; /*!< Equivalent of realloc(). */
    VmnlNetAllocatorDeallocate deallocate; /*!< Equivalent of free(). */
} VmnlNetAllocator;

/**
 * \brief Global allocator of every allocation of the library.
 *
 * It starts with malloc(), realloc() and free(). On Windows, it calls them through static functions, as MSVC warns about the address of a function of the C runtime DLL in a constant initializer (C4232).
 */
extern VmnlNetAllocator vmnl_net_allocator;

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ALLOCATOR_INTERNAL_H */

/** @} */
