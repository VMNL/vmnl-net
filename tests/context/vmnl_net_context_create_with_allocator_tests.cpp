#include <vmnl/net/context.h>

#include <gtest/gtest.h>

#include <cstdlib>

static size_t allocations;

class VmnlNetContextCreateWithAllocator : public testing::Test {
  protected:
    void SetUp() override
    {
        allocations = 0;
    }
};

TEST_F(VmnlNetContextCreateWithAllocator, CustomAllocator)
{
    VmnlNetAllocator allocator = {
        [](size_t size) { allocations++; return std::malloc(size); },
        std::realloc,
        std::free,
    };
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&allocator, &error);

    EXPECT_NE(context, nullptr);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_GT(allocations, 0);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetContextCreateWithAllocator, NullAllocator)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_context_create_with_allocator(nullptr, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetContextCreateWithAllocator, NullAllocate)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAllocator allocator = {nullptr, std::realloc, std::free};

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetContextCreateWithAllocator, NullReallocate)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAllocator allocator = {std::malloc, nullptr, std::free};

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetContextCreateWithAllocator, NullDeallocate)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAllocator allocator = {std::malloc, std::realloc, nullptr};

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetContextCreateWithAllocator, AllocationFailure)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAllocator allocator = {[](size_t) -> void * { return nullptr; }, std::realloc, std::free};

    EXPECT_EQ(vmnl_net_context_create_with_allocator(&allocator, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
}

TEST_F(VmnlNetContextCreateWithAllocator, NullError)
{
    EXPECT_EQ(vmnl_net_context_create_with_allocator(nullptr, nullptr), nullptr);
}
