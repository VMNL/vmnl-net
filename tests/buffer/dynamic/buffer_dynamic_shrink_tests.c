#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_dynamic_shrink, VMNL_NET_BUFFER_DYNAMIC, 8)

UTEST_F(vmnl_net_buffer_dynamic_shrink, shrinks_to_exact_capacity)
{
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 7, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 7);
}

UTEST_F(vmnl_net_buffer_dynamic_shrink, moves_stored_bytes_to_front)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 2, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "ef"));
}

UTEST_F(vmnl_net_buffer_dynamic_shrink, frees_every_byte_with_zero_capacity)
{
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 0, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 0);
}

UTEST_F(vmnl_net_buffer_dynamic_shrink, allocates_again_after_freeing_every_byte)
{
    ASSERT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 0, NULL));
    EXPECT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_F(vmnl_net_buffer_dynamic_shrink, fails_without_memory)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    allocator_limit(0);
    EXPECT_FALSE(vmnl_net_buffer_shrink(utest_fixture->buffer, 4, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 8);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "abc"));
}
