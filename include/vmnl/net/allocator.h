/**
 * \file allocator.h
 *
 * \defgroup Allocator Allocator
 * \addtogroup Allocator
 * \brief Memory allocator of the library.
 *
 * Every allocation of the library goes through one global allocator: malloc(), realloc() and free() until vmnl_net_allocator_set() replaces them.
 *
 * \par Example
 * \code
 * // custom memory allocator
 * void *my_malloc(size_t size);
 *
 * // custom memory reallocator
 * void *my_realloc(void *pointer, size_t size);
 *
 * // custom memory deallocator
 * void my_free(void *pointer);
 *
 * int main(void)
 * {
 *     vmnl_net_allocator_set(my_malloc, my_realloc, my_free, NULL); // before any other call
 *     app_run();
 *     return 0;
 * }
 * \endcode
 * @{
 */

#ifndef VMNL_NET_ALLOCATOR_H
#define VMNL_NET_ALLOCATOR_H

#include <vmnl/net/error.h>

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Sets the allocator of the whole library.
 * \param allocate Custom memory allocator.
 * \param reallocate Custom memory reallocator.
 * \param deallocate Custom memory deallocator.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: one of the 3 functions is NULL.
 * \return true on success, false otherwise.
 * \attention It **MUST** be called before any other function, or once no object is alive: an object allocated by the previous allocator would be freed by the new one.
 * \attention It **MUST** be called while no other thread uses the library: the allocator is a global variable, written without lock.
 * \attention The functions **MUST** be thread-safe and keep the contract of their standard counterpart, alignment included: the library stores any type in the memory they return.
 * \remark Passing malloc, realloc and free restores the default allocator.
 */
VMNL_NET_API bool vmnl_net_allocator_set(void *(*allocate)(size_t size), void *(*reallocate)(void *pointer, size_t size), void (*deallocate)(void *pointer), VmnlNetError *error);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ALLOCATOR_H */

/** @} */
