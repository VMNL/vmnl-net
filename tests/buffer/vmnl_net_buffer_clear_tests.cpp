#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

TEST(VmnlNetBufferClear, KeepsCapacity)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_ring(context, 64, nullptr);

    vmnl_net_buffer_clear(buffer);
    EXPECT_EQ(vmnl_net_buffer_size(buffer), 0);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), 64);
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
}

TEST(VmnlNetBufferClear, NullBuffer)
{
    vmnl_net_buffer_clear(nullptr);
}
