#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

#include <cstdlib>

class VmnlNetBufferReserve : public testing::Test {
  protected:
    void SetUp() override
    {
        context = vmnl_net_context_create(nullptr);
        dynamic_buffer = vmnl_net_buffer_create_dynamic(context, 0, nullptr);
        ring_buffer = vmnl_net_buffer_create_ring(context, 64, nullptr);
    }

    void TearDown() override
    {
        vmnl_net_buffer_destroy(context, dynamic_buffer);
        vmnl_net_buffer_destroy(context, ring_buffer);
        vmnl_net_context_destroy(context);
    }

    VmnlNetContext *context;
    VmnlNetBuffer *dynamic_buffer;
    VmnlNetBuffer *ring_buffer;
};

TEST_F(VmnlNetBufferReserve, Success)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_reserve(context, dynamic_buffer, 100, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(dynamic_buffer), 100);
}

TEST_F(VmnlNetBufferReserve, SmallerCapacity)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_reserve(context, dynamic_buffer, 100, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_reserve(context, dynamic_buffer, 50, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(dynamic_buffer), 100);
}

TEST_F(VmnlNetBufferReserve, RingWithinCapacity)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_reserve(context, ring_buffer, 64, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

TEST_F(VmnlNetBufferReserve, RingBeyondCapacity)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_reserve(context, ring_buffer, 65, &error));
    EXPECT_EQ(error, VMNL_NET_ENOBUFS);
    EXPECT_EQ(vmnl_net_buffer_capacity(ring_buffer), 64);
}

TEST_F(VmnlNetBufferReserve, ReallocationFailure)
{
    VmnlNetAllocator allocator = {std::malloc, [](void *, size_t) -> void * { return nullptr; }, std::free};
    VmnlNetContext *f_context = vmnl_net_context_create_with_allocator(&allocator, nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(f_context, 0, nullptr);
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_reserve(f_context, buffer, 100, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), 0);
    vmnl_net_buffer_destroy(f_context, buffer);
    vmnl_net_context_destroy(f_context);
}

TEST_F(VmnlNetBufferReserve, NullContext)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_reserve(nullptr, dynamic_buffer, 100, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferReserve, NullBuffer)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_reserve(context, nullptr, 100, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
