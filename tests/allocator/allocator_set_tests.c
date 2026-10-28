#include <vmnl/net/allocator.h>
#include <vmnl/net/buffer.h>

#include "utest.h"

static size_t allocations;
static size_t reallocations;
static size_t deallocations;

static void *malloc_stub(size_t size)
{
    allocations++;
    return malloc(size);
}

static void *realloc_stub(void *pointer, size_t size)
{
    reallocations++;
    return realloc(pointer, size);
}

static void free_stub(void *pointer)
{
    deallocations++;
    free(pointer);
}

UTEST(vmnl_net_allocator_set, replaces_callbacks)
{
    VmnlNetBuffer *buffer;

    vmnl_net_allocator_set(malloc_stub, realloc_stub, free_stub);
    buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_DYNAMIC, 0, NULL);
    vmnl_net_buffer_resize(buffer, 16, NULL);
    vmnl_net_buffer_destroy(buffer);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(allocations, (size_t) 1);
    EXPECT_EQ(reallocations, (size_t) 1);
    EXPECT_EQ(deallocations, (size_t) 2);
}
