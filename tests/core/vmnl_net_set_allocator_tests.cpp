#include <vmnl/net.h>

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

TEST(VmnlNetSetAllocator, NullAllocate)
{
    EXPECT_EQ(vmnl_net_set_allocator(nullptr, std::realloc, std::free), VMNL_NET_EINVAL);
}

TEST(VmnlNetSetAllocator, NullReallocate)
{
    EXPECT_EQ(vmnl_net_set_allocator(std::malloc, nullptr, std::free), VMNL_NET_EINVAL);
}

TEST(VmnlNetSetAllocator, NullDeallocate)
{
    EXPECT_EQ(vmnl_net_set_allocator(std::malloc, std::realloc, nullptr), VMNL_NET_EINVAL);
}

TEST(VmnlNetSetAllocator, CustomFunctions)
{
    VmnlNetContext *context = nullptr;

    allocations   = 0;
    deallocations = 0;
    ASSERT_EQ(vmnl_net_set_allocator(counting_allocate, std::realloc, counting_deallocate), VMNL_NET_SUCCESS);
    ASSERT_EQ(vmnl_net_context_create(&context), VMNL_NET_SUCCESS);
    EXPECT_GT(allocations, 0);
    vmnl_net_context_destroy(context);
    EXPECT_GT(deallocations, 0);
    vmnl_net_set_allocator(std::malloc, std::realloc, std::free);
}
