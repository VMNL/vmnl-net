#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

#include <cstring>

namespace {

class VmnlNetBufferView : public testing::Test {
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

TEST_F(VmnlNetBufferView, Success)
{
    ::VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abcdef", 6, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_view(dynamic_buffer, views, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    ASSERT_EQ(views[0].size, 6);
    EXPECT_EQ(std::memcmp(views[0].data, "abcdef", 6), 0);
    EXPECT_EQ(views[1].size, 0);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 6);
}

TEST_F(VmnlNetBufferView, RingWrapsAround)
{
    ::VmnlNetBufferView views[2];

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(ring_buffer, 4, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "ghij", 4, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_view(ring_buffer, views, nullptr));
    ASSERT_EQ(views[0].size, 4);
    EXPECT_EQ(std::memcmp(views[0].data, "efgh", 4), 0);
    ASSERT_EQ(views[1].size, 2);
    EXPECT_EQ(std::memcmp(views[1].data, "ij", 2), 0);
}

TEST_F(VmnlNetBufferView, ModifiesBytesInPlace)
{
    ::VmnlNetBufferView views[2];
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abc", 3, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_view(dynamic_buffer, views, nullptr));
    views[0].data[0] = 'x';
    ASSERT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, data, 3, nullptr));
    EXPECT_STREQ(data, "xbc");
}

TEST_F(VmnlNetBufferView, EmptyBuffer)
{
    ::VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_view(dynamic_buffer, views, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(views[0].size, 0);
    EXPECT_EQ(views[1].size, 0);
}

TEST_F(VmnlNetBufferView, NullBuffer)
{
    ::VmnlNetBufferView views[2];
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_view(nullptr, views, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferView, NullViews)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_view(dynamic_buffer, nullptr, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

} // namespace
