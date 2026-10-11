#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

#include <cstdlib>

static size_t allocations = 0;
static size_t deallocations = 0;

class VmnlNetBufferCreateRing : public testing::Test {
  protected:
    void SetUp() override
    {
        allocations = 0;
        deallocations = 0;
    }
};

TEST_F(VmnlNetBufferCreateRing, Success)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetError error = UINT32_MAX;
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_ring(context, 64, &error);

    EXPECT_NE(buffer, nullptr);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), 64);
    EXPECT_EQ(vmnl_net_buffer_size(buffer), 0);
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetBufferCreateRing, ZeroCapacity)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_buffer_create_ring(context, 0, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetBufferCreateRing, NullContext)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_buffer_create_ring(nullptr, 64, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferCreateRing, AllocationFailure)
{
    VmnlNetAllocator allocator = {[](size_t size) { return allocations++ == 1 ? nullptr : std::malloc(size); }, std::realloc, std::free};
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&allocator, nullptr);
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_buffer_create_ring(context, 64, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetBufferCreateRing, StorageAllocationFailure)
{
    VmnlNetAllocator allocator = {
        [](size_t size) { allocations++; return std::malloc(size); },
        [](void *, size_t) -> void * { return nullptr; },
        [](void *pointer) { deallocations++; std::free(pointer); },
    };
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&allocator, nullptr);
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_buffer_create_ring(context, 64, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    vmnl_net_context_destroy(context);
    EXPECT_EQ(allocations, deallocations);
}

TEST_F(VmnlNetBufferCreateRing, NullError)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_ring(context, 64, nullptr);

    EXPECT_NE(buffer, nullptr);
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
}
