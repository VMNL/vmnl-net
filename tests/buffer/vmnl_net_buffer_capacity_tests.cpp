#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

TEST(VmnlNetBufferCapacity, Success)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_ring(context, 64, nullptr);

    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), 64);
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
}

TEST(VmnlNetBufferCapacity, NullBuffer)
{
    EXPECT_EQ(vmnl_net_buffer_capacity(nullptr), 0);
}
