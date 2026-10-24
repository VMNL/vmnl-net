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
    EXPECT_EQ((const void *) vmnl_net_allocator.allocate, (const void *) malloc_stub);
    EXPECT_EQ((const void *) vmnl_net_allocator.reallocate, (const void *) realloc_stub);
    EXPECT_EQ((const void *) vmnl_net_allocator.deallocate, (const void *) free_stub);
    vmnl_net_allocator_set(malloc, realloc, free);
}
