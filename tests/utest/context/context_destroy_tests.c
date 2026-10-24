#include <vmnl/net/context.h>
#include <vmnl/net/allocator.h>

#include "utest.h"

static size_t deallocations;

static void deallocator(void *pointer)
{
    deallocations++;
    free(pointer);
}

UTEST(vmnl_net_context_destroy, frees_context)
{
    VmnlNetContext *context;

    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    context = vmnl_net_context_create(NULL);
    EXPECT_NE(context, NULL);
    vmnl_net_context_destroy(context);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ((int) deallocations, 1);
}

UTEST(vmnl_net_context_destroy, ignores_null_context)
{
    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    vmnl_net_context_destroy(NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ((int) deallocations, 0);
}
