/**
 * \file allocator.h
 *
 * \defgroup Allocator Allocator
 * \addtogroup Allocator
 * \brief Replaceable global allocator.
 *
 * Every allocation of the library goes through one global allocator, which uses \iso{malloc,memory}, \iso{realloc,memory} and \iso{free,memory} until vmnl_net_allocator_set() replaces them.
 *
 * \par Example
 * Example to be written.
 * @{
 */

#ifndef VMNL_NET_ALLOCATOR_H
#define VMNL_NET_ALLOCATOR_H

#include <vmnl/net_api.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Function that allocates memory, following malloc()'s contract.
 * \param size Number of bytes to allocate.
 * \return Allocated memory on success, NULL otherwise.
 */
typedef void *(*VmnlNetAllocatorAllocate)(size_t size);

/**
 * \brief Function that reallocates memory, following realloc()'s contract.
 * \param pointer Memory to reallocate, nullable.
 * \param size Number of bytes to reallocate to.
 * \return Reallocated memory on success, NULL otherwise.
 */
typedef void *(*VmnlNetAllocatorReallocate)(void *pointer, size_t size);

/**
 * \brief Function that releases memory, following free()'s contract.
 * \param pointer Memory to release, nullable.
 */
typedef void (*VmnlNetAllocatorDeallocate)(void *pointer);

/**
 * \brief Replaces the global allocator's functions.
 * \param allocate Function allocating memory.
 * \param reallocate Function reallocating memory.
 * \param deallocate Function releasing memory.
 * \attention
 * - This function **MUST** be called before any other function of the library, or while no object of the library is alive, as an object allocated by the previous allocator would be freed by the new one.
 * - This function **MUST** be called while no other thread uses the library, as the allocator has no locks.
 * - The 3 functions **MUST** be thread-safe when several threads use the library, as every thread allocates through this global allocator.
 * \remark Passing malloc, realloc and free restores the default allocator.
 */
VMNL_NET_API void vmnl_net_allocator_set(VmnlNetAllocatorAllocate allocate, VmnlNetAllocatorReallocate reallocate, VmnlNetAllocatorDeallocate deallocate);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ALLOCATOR_H */

/** @} */
