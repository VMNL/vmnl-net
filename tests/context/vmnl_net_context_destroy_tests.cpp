#include <vmnl/net/context.h>

#include <gtest/gtest.h>

#include <cstdlib>

static size_t allocations = 0;
static size_t deallocations = 0;

class VmnlNetContextDestroy : public testing::Test {
  protected:
    void SetUp() override
    {
        allocations = 0;
        deallocations = 0;
    }
};

TEST_F(VmnlNetContextDestroy, ReleasesMemory)
{
    VmnlNetAllocator allocator = {
        [](size_t size) { allocations++; return std::malloc(size); },
        std::realloc,
        [](void *pointer) { deallocations++; std::free(pointer); },
    };
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&allocator, nullptr);

    ASSERT_NE(context, nullptr);
    vmnl_net_context_destroy(context);
    EXPECT_EQ(allocations, deallocations);
}

TEST_F(VmnlNetContextDestroy, NullContext)
{
    vmnl_net_context_destroy(nullptr);
}
