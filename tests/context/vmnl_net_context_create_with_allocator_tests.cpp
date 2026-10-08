#include <vmnl/net/context.h>

#include <gtest/gtest.h>

#include <cstdlib>

static int allocations;

static void *counting_allocate(size_t size)
{
    ++allocations;
    return std::malloc(size);
}

static void *failing_allocate(size_t)
{
    return nullptr;
}

TEST(VmnlNetContextCreateWithAllocator, CustomFunctions)
{
    VmnlNetAllocator allocator = {counting_allocate, std::realloc, std::free};
    VmnlNetError error         = UINT32_MAX;
    VmnlNetContext *context;

    allocations = 0;
    context     = vmnl_net_context_create_with_allocator(&allocator, &error);
    EXPECT_NE(context, nullptr);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_GT(allocations, 0);
    vmnl_net_context_destroy(context);
}

TEST(VmnlNetContextCreateWithAllocator, NullAllocator)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_context_create_with_allocator(nullptr, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST(VmnlNetContextCreateWithAllocator, NullAllocate)
{
    VmnlNetAllocator allocator = {nullptr, std::realloc, std::free};
    VmnlNetError error         = UINT32_MAX;

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST(VmnlNetContextCreateWithAllocator, NullReallocate)
{
    VmnlNetAllocator allocator = {std::malloc, nullptr, std::free};
    VmnlNetError error         = UINT32_MAX;

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST(VmnlNetContextCreateWithAllocator, NullDeallocate)
{
    VmnlNetAllocator allocator = {std::malloc, std::realloc, nullptr};
    VmnlNetError error         = UINT32_MAX;

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST(VmnlNetContextCreateWithAllocator, AllocationFailure)
{
    VmnlNetAllocator allocator = {failing_allocate, std::realloc, std::free};
    VmnlNetError error         = UINT32_MAX;

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
}

TEST(VmnlNetContextCreateWithAllocator, NullError)
{
    EXPECT_EQ(vmnl_net_context_create_with_allocator(nullptr, nullptr), nullptr);
}
