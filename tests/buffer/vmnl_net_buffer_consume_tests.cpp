#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

class VmnlNetBufferConsume : public testing::Test {
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

TEST_F(VmnlNetBufferConsume, Success)
{
    VmnlNetError error = UINT32_MAX;
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abcdef", 6, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_consume(dynamic_buffer, 4, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 2);
    ASSERT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, data, 2, nullptr));
    EXPECT_STREQ(data, "ef");
}

TEST_F(VmnlNetBufferConsume, RingWrapsAround)
{
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdefgh", 8, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(ring_buffer, 6, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "ij", 2, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_consume(ring_buffer, 3, nullptr));
    EXPECT_EQ(vmnl_net_buffer_size(ring_buffer), 1);
    ASSERT_TRUE(vmnl_net_buffer_peek(ring_buffer, data, 1, nullptr));
    EXPECT_STREQ(data, "j");
}

TEST_F(VmnlNetBufferConsume, EmptiesTheBuffer)
{
    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_consume(ring_buffer, 6, nullptr));
    EXPECT_EQ(vmnl_net_buffer_size(ring_buffer), 0);
    EXPECT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdefgh", 8, nullptr));
}

TEST_F(VmnlNetBufferConsume, BeyondStoredBytes)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abc", 3, nullptr));
    EXPECT_FALSE(vmnl_net_buffer_consume(dynamic_buffer, 4, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 3);
}

TEST_F(VmnlNetBufferConsume, NullBuffer)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_consume(nullptr, 1, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
