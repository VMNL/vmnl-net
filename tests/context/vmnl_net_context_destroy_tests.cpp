#include <vmnl/net/context.h>

#include <gtest/gtest.h>

#include <cstdlib>

static int allocations;
static int deallocations;

static void *counting_allocate(size_t size)
{
    ++allocations;
    return std::malloc(size);
}

static void counting_deallocate(void *pointer)
{
    ++deallocations;
    std::free(pointer);
}

TEST(VmnlNetContextDestroy, ReleasesMemory)
{
    VmnlNetContext *context = nullptr;

    allocations   = 0;
    deallocations = 0;
    ASSERT_EQ(vmnl_net_set_allocator(counting_allocate, std::realloc, counting_deallocate), VMNL_NET_SUCCESS);
    ASSERT_EQ(vmnl_net_context_create(&context), VMNL_NET_SUCCESS);
    vmnl_net_context_destroy(context);
    EXPECT_EQ(deallocations, allocations);
    vmnl_net_set_allocator(std::malloc, std::realloc, std::free);
}

TEST(VmnlNetContextDestroy, NullContext)
{
    deallocations = 0;
    ASSERT_EQ(vmnl_net_set_allocator(std::malloc, std::realloc, counting_deallocate), VMNL_NET_SUCCESS);
    vmnl_net_context_destroy(nullptr);
    EXPECT_EQ(deallocations, 0);
    vmnl_net_set_allocator(std::malloc, std::realloc, std::free);
}
