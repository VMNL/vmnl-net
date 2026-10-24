#include "allocator/allocator_internal.h"

#include "utest.h"

static void *malloc_stub(size_t size)
{
    return malloc(size);
}

static void *realloc_stub(void *pointer, size_t size)
{
    return realloc(pointer, size);
}

static void free_stub(void *pointer)
{
    free(pointer);
}

UTEST(vmnl_net_allocator_set, uses_given_functions)
{
    vmnl_net_allocator_set(malloc_stub, realloc_stub, free_stub);
    EXPECT_EQ((uintptr_t) vmnl_net_allocator.allocate, (uintptr_t) malloc_stub);
    EXPECT_EQ((uintptr_t) vmnl_net_allocator.reallocate, (uintptr_t) realloc_stub);
    EXPECT_EQ((uintptr_t) vmnl_net_allocator.deallocate, (uintptr_t) free_stub);
    vmnl_net_allocator_set(malloc, realloc, free);
}
