/**
 * \file allocator_internal.h
 * \brief Global allocator of the library.
 *
 * \par Example
 * \code
 * VmnlNetContext *context = vmnl_net_allocator.allocate(sizeof *context);
 *
 * if (context == NULL) {
 *     vmnl_net_error_set(error, VMNL_NET_ENOMEM);
 *     return NULL;
 * }
 * vmnl_net_allocator.deallocate(context);
 * \endcode
 */

#ifndef VMNL_NET_ALLOCATOR_INTERNAL_H
#define VMNL_NET_ALLOCATOR_INTERNAL_H

#include <vmnl/net/allocator.h> /* IWYU pragma: export */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Memory allocation functions.
 */
typedef struct {
    void *(*allocate)(size_t size); /*!< Equivalent of malloc(). */
    void *(*reallocate)(void *pointer, size_t size); /*!< Equivalent of realloc(). */
    void (*deallocate)(void *pointer); /*!< Equivalent of free(). */
} VmnlNetAllocator;

/**
 * \brief Allocator used by every allocation of the library.
 * \remark malloc(), realloc() and free() until vmnl_net_allocator_set() replaces them, read without lock.
 */
extern VmnlNetAllocator vmnl_net_allocator;

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ALLOCATOR_INTERNAL_H */
