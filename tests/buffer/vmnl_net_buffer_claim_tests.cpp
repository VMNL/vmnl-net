#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

#include <cstdlib>

class VmnlNetBufferClaim : public testing::Test {
  protected:
    void SetUp() override
    {
        context = vmnl_net_context_create(nullptr);
        dynamic_buffer = vmnl_net_buffer_create_dynamic(context, 0, nullptr);
        ring_buffer = vmnl_net_buffer_create_ring(context, 8, nullptr);
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

TEST_F(VmnlNetBufferClaim, Success)
{
    VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_claim(context, dynamic_buffer, 4, views, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_GE(views[0].size, 4);
    EXPECT_EQ(views[1].size, 0);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 0);
}

TEST_F(VmnlNetBufferClaim, RingWrapsAround)
{
    VmnlNetBufferView views[2];

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(ring_buffer, 4, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_claim(context, ring_buffer, 4, views, nullptr));
    EXPECT_EQ(views[0].size, 2);
    EXPECT_EQ(views[1].size, 4);
}

TEST_F(VmnlNetBufferClaim, ZeroSize)
{
    VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_claim(context, dynamic_buffer, 0, views, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(views[0].size, 0);
    EXPECT_EQ(views[1].size, 0);
}

TEST_F(VmnlNetBufferClaim, RingBeyondCapacity)
{
    VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    EXPECT_FALSE(vmnl_net_buffer_claim(context, ring_buffer, 3, views, &error));
    EXPECT_EQ(error, VMNL_NET_ENOBUFS);
}

TEST_F(VmnlNetBufferClaim, ReallocationFailure)
{
    VmnlNetAllocator allocator = {std::malloc, [](void *, size_t) -> void * { return nullptr; }, std::free};
    VmnlNetContext *f_context = vmnl_net_context_create_with_allocator(&allocator, nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(f_context, 0, nullptr);
    VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_claim(f_context, buffer, 4, views, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    vmnl_net_buffer_destroy(f_context, buffer);
    vmnl_net_context_destroy(f_context);
}

TEST_F(VmnlNetBufferClaim, NullContext)
{
    VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_claim(nullptr, dynamic_buffer, 4, views, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferClaim, NullBuffer)
{
    VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_claim(context, nullptr, 4, views, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferClaim, NullViews)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_claim(context, dynamic_buffer, 4, nullptr, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
