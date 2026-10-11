#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

TEST(VmnlNetBufferSize, Success)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(context, 64, nullptr);

    EXPECT_EQ(vmnl_net_buffer_size(buffer), 0);
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
}

TEST(VmnlNetBufferSize, NullBuffer)
{
    EXPECT_EQ(vmnl_net_buffer_size(nullptr), 0);
}
