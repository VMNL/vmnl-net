#include <vmnl/net/buffer.h>

#include <gtest/gtest.h>

#include <cstdlib>

class VmnlNetBufferWrite : public testing::Test {
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

TEST_F(VmnlNetBufferWrite, Success)
{
    VmnlNetError error = UINT32_MAX;
    char data[16] = {};

    EXPECT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abc", 3, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 3);
    ASSERT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, data, 3, nullptr));
    EXPECT_STREQ(data, "abc");
}

TEST_F(VmnlNetBufferWrite, GrowsByHalf)
{
    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abc", 3, nullptr));
    EXPECT_EQ(vmnl_net_buffer_capacity(dynamic_buffer), 3);
    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "de", 2, nullptr));
    EXPECT_EQ(vmnl_net_buffer_capacity(dynamic_buffer), 5);
    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "f", 1, nullptr));
    EXPECT_EQ(vmnl_net_buffer_capacity(dynamic_buffer), 7);
}

TEST_F(VmnlNetBufferWrite, MovesBytesToTheFront)
{
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abcdef", 6, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(dynamic_buffer, 2, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "gh", 2, nullptr));
    EXPECT_EQ(vmnl_net_buffer_capacity(dynamic_buffer), 6);
    ASSERT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, data, 6, nullptr));
    EXPECT_STREQ(data, "cdefgh");
}

TEST_F(VmnlNetBufferWrite, KeepsBytesInPlace)
{
    VmnlNetError error = UINT32_MAX;
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_reserve(context, dynamic_buffer, 8, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "abcd", 4, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(dynamic_buffer, 2, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, "ef", 2, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(dynamic_buffer), 8);
    ASSERT_TRUE(vmnl_net_buffer_peek(dynamic_buffer, data, 4, nullptr));
    EXPECT_STREQ(data, "cdef");
}

TEST_F(VmnlNetBufferWrite, RingWrapsAround)
{
    char data[16] = {};

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    ASSERT_TRUE(vmnl_net_buffer_consume(ring_buffer, 4, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "ghij", 4, nullptr));
    EXPECT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "kl", 2, nullptr));
    EXPECT_EQ(vmnl_net_buffer_size(ring_buffer), 8);
    ASSERT_TRUE(vmnl_net_buffer_peek(ring_buffer, data, 8, nullptr));
    EXPECT_STREQ(data, "efghijkl");
}

TEST_F(VmnlNetBufferWrite, ZeroSize)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_write(context, dynamic_buffer, nullptr, 0, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_size(dynamic_buffer), 0);
}

TEST_F(VmnlNetBufferWrite, RingBeyondCapacity)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(vmnl_net_buffer_write(context, ring_buffer, "abcdef", 6, nullptr));
    EXPECT_FALSE(vmnl_net_buffer_write(context, ring_buffer, "ghi", 3, &error));
    EXPECT_EQ(error, VMNL_NET_ENOBUFS);
    EXPECT_EQ(vmnl_net_buffer_size(ring_buffer), 6);
}

TEST_F(VmnlNetBufferWrite, ReallocationFailure)
{
    VmnlNetAllocator allocator = {std::malloc, [](void *, size_t) -> void * { return nullptr; }, std::free};
    VmnlNetContext *f_context = vmnl_net_context_create_with_allocator(&allocator, nullptr);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(f_context, 0, nullptr);
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_write(f_context, buffer, "abc", 3, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_size(buffer), 0);
    vmnl_net_buffer_destroy(f_context, buffer);
    vmnl_net_context_destroy(f_context);
}

TEST_F(VmnlNetBufferWrite, NullContext)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_write(nullptr, dynamic_buffer, "abc", 3, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferWrite, NullBuffer)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_write(context, nullptr, "abc", 3, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetBufferWrite, NullData)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_write(context, dynamic_buffer, nullptr, 3, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
