#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

#include <cstdlib>

static size_t allocations = 0;
static size_t deallocations = 0;

class VmnlNetBufferDestroy : public testing::Test {
  protected:
    void SetUp() override
    {
        allocations = 0;
        deallocations = 0;
    }
};

static VmnlNetAllocator counting_allocator = {
    [](size_t size) { allocations++; return std::malloc(size); },
    [](void *pointer, size_t size) { if (pointer == nullptr) { allocations++; } return std::realloc(pointer, size); },
    [](void *pointer) { if (pointer != nullptr) { deallocations++; } std::free(pointer); },
};

TEST_F(VmnlNetBufferDestroy, ReleasesMemory)
{
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&counting_allocator, nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(context, 64, nullptr);

    ASSERT_NE(buffer, nullptr);
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
    EXPECT_EQ(allocations, deallocations);
}

TEST_F(VmnlNetBufferDestroy, NullContext)
{
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&counting_allocator, nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_ring(context, 64, nullptr);
    size_t released = deallocations;

    vmnl_net_buffer_destroy(nullptr, buffer);
    EXPECT_EQ(deallocations, released);
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetBufferDestroy, NullBuffer)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);

    vmnl_net_buffer_destroy(context, nullptr);
    vmnl_net_context_destroy(context);
}
