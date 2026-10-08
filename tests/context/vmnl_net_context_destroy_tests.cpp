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
    VmnlNetAllocator allocator = {counting_allocate, std::realloc, counting_deallocate};
    VmnlNetContext *context;

    allocations   = 0;
    deallocations = 0;
    context       = vmnl_net_context_create_with_allocator(&allocator, nullptr);
    ASSERT_NE(context, nullptr);
    vmnl_net_context_destroy(context);
    EXPECT_EQ(deallocations, allocations);
}

TEST(VmnlNetContextDestroy, NullContext)
{
    vmnl_net_context_destroy(nullptr);
}
