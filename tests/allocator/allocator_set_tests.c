#include <vmnl/net/allocator.h>
#include <vmnl/net/context.h>

#include "utest.h"

#include <stdint.h>
#include <stdlib.h>

static int nallocations;
static int nreallocations;
static int ndeallocations;

static void *count_allocation(size_t size)
{
    nallocations++;
    return malloc(size);
}

static void *count_reallocation(void *pointer, size_t size)
{
    nreallocations++;
    return realloc(pointer, size);
}

static void count_deallocation(void *pointer)
{
    ndeallocations++;
    free(pointer);
}

static void reset_allocation_counters(void)
{
    nallocations = 0;
    nreallocations = 0;
    ndeallocations = 0;
}

UTEST(vmnl_net_allocator_set, success)
{
    VmnlNetError error = UINT32_MAX;

    reset_allocation_counters();
    EXPECT_EQ(vmnl_net_allocator_set(count_allocation, count_reallocation, count_deallocation, &error), true);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    vmnl_net_context_destroy(vmnl_net_context_create(NULL));
    vmnl_net_allocator_set(malloc, realloc, free, NULL);
    EXPECT_EQ(nallocations, 1);
    EXPECT_EQ(ndeallocations, 1);
}

UTEST(vmnl_net_allocator_set, reallocates_with_the_given_function)
{
    UTEST_SKIP("needs a dynamic buffer, the first code that reallocates");
}

UTEST(vmnl_net_allocator_set, null_allocate)
{
    VmnlNetError error = UINT32_MAX;

    reset_allocation_counters();
    vmnl_net_allocator_set(count_allocation, count_reallocation, count_deallocation, NULL);
    EXPECT_EQ(vmnl_net_allocator_set(NULL, realloc, free, &error), false);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    vmnl_net_context_destroy(vmnl_net_context_create(NULL));
    vmnl_net_allocator_set(malloc, realloc, free, NULL);
    EXPECT_EQ(nallocations, 1);
    EXPECT_EQ(ndeallocations, 1);
}

UTEST(vmnl_net_allocator_set, null_reallocate)
{
    VmnlNetError error = UINT32_MAX;

    reset_allocation_counters();
    vmnl_net_allocator_set(count_allocation, count_reallocation, count_deallocation, NULL);
    EXPECT_EQ(vmnl_net_allocator_set(malloc, NULL, free, &error), false);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    vmnl_net_context_destroy(vmnl_net_context_create(NULL));
    vmnl_net_allocator_set(malloc, realloc, free, NULL);
    EXPECT_EQ(nallocations, 1);
    EXPECT_EQ(ndeallocations, 1);
}

UTEST(vmnl_net_allocator_set, null_deallocate)
{
    VmnlNetError error = UINT32_MAX;

    reset_allocation_counters();
    vmnl_net_allocator_set(count_allocation, count_reallocation, count_deallocation, NULL);
    EXPECT_EQ(vmnl_net_allocator_set(malloc, realloc, NULL, &error), false);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    vmnl_net_context_destroy(vmnl_net_context_create(NULL));
    vmnl_net_allocator_set(malloc, realloc, free, NULL);
    EXPECT_EQ(nallocations, 1);
    EXPECT_EQ(ndeallocations, 1);
}

UTEST(vmnl_net_allocator_set, null_error)
{
    EXPECT_EQ(vmnl_net_allocator_set(malloc, realloc, free, NULL), true);
}
