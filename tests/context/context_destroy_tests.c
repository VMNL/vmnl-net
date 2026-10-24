#include <vmnl/net/allocator.h>
#include <vmnl/net/context.h>

#include "utest.h"

#include <stddef.h>
#include <stdlib.h>

static int ndeallocations;

static void count_deallocation(void *pointer)
{
    ndeallocations++;
    free(pointer);
}

UTEST(vmnl_net_context_destroy, success)
{
    VmnlNetContext *context;

    ndeallocations = 0;
    vmnl_net_allocator_set(malloc, realloc, count_deallocation);
    context = vmnl_net_context_create(NULL);
    EXPECT_NE(context, NULL);
    vmnl_net_context_destroy(context);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(ndeallocations, 1);
}

UTEST(vmnl_net_context_destroy, null_context)
{
    ndeallocations = 0;
    vmnl_net_allocator_set(malloc, realloc, count_deallocation);
    vmnl_net_context_destroy(NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(ndeallocations, 0);
}
