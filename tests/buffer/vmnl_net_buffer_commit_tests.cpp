#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

#include <cstring>

class VmnlNetBufferCommit : public testing::Test {
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

TEST_F(VmnlNetBufferCommit, Success)
{
    VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_claim(context, dynamic_buffer, 4, views, nullptr));
    std::memcpy(views[0].data, "abcd", 4);
    EXPECT_TRUE(vmnl_net_buffer_commit(dynamic_buffer, 4, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 4);
    ASSERT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, data, 4, nullptr));
    EXPECT_STREQ(data, "abcd");
}

TEST_F(VmnlNetBufferCommit, RingWrapsAround)
{
    VmnlNetBufferView views[2];
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(ring_buffer, 4, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_claim(context, ring_buffer, 4, views, nullptr));
    std::memcpy(views[0].data, "gh", 2);
    std::memcpy(views[1].data, "ij", 2);
    EXPECT_TRUE(vmnl_net_buffer_commit(ring_buffer, 4, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_peek(ring_buffer, data, 6, nullptr));
    EXPECT_STREQ(data, "efghij");
}

TEST_F(VmnlNetBufferCommit, ZeroSize)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_commit(dynamic_buffer, 0, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 0);
}

TEST_F(VmnlNetBufferCommit, BeyondFreeSpace)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    EXPECT_FALSE(vmnl_net_buffer_commit(ring_buffer, 3, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_EQ(vmnl_net_buffer_size(ring_buffer), 6);
}

TEST_F(VmnlNetBufferCommit, DynamicBeyondItsEnd)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_reserve(context, dynamic_buffer, 8, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abcdefgh", 8, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(dynamic_buffer, 2, nullptr));
    EXPECT_FALSE(vmnl_net_buffer_commit(dynamic_buffer, 1, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 6);
}

TEST_F(VmnlNetBufferCommit, NullBuffer)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_commit(nullptr, 4, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
