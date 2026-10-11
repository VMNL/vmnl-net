#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

class VmnlNetBufferPeek : public testing::Test {
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

TEST_F(VmnlNetBufferPeek, Success)
{
    VmnlNetError error = UINT32_MAX;
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abcdef", 6, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, data, 4, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_STREQ(data, "abcd");
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 6);
}

TEST_F(VmnlNetBufferPeek, RingWrapsAround)
{
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(ring_buffer, 4, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "ghij", 4, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_peek(ring_buffer, data, 6, nullptr));
    EXPECT_STREQ(data, "efghij");
}

TEST_F(VmnlNetBufferPeek, ZeroSize)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, nullptr, 0, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

TEST_F(VmnlNetBufferPeek, BeyondStoredBytes)
{
    VmnlNetError error = UINT32_MAX;
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abc", 3, nullptr));
    EXPECT_FALSE(vmnl_net_buffer_peek(dynamic_buffer, data, 4, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 3);
}

TEST_F(VmnlNetBufferPeek, NullBuffer)
{
    VmnlNetError error = UINT32_MAX;
    char data[16] = {};

    EXPECT_FALSE(vmnl_net_buffer_peek(nullptr, data, 4, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferPeek, NullData)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abc", 3, nullptr));
    EXPECT_FALSE(vmnl_net_buffer_peek(dynamic_buffer, nullptr, 3, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
