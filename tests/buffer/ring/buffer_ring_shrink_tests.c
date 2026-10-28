#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_ring_shrink, VMNL_NET_BUFFER_RING, 8)

UTEST_F(vmnl_net_buffer_ring_shrink, shrinks_to_exact_capacity)
{
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 7, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 7);
}

UTEST_F(vmnl_net_buffer_ring_shrink, unwraps_stored_bytes)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghi"));
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 4, NULL));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 4);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "fghi"));
}

UTEST_F(vmnl_net_buffer_ring_shrink, rejects_zero_capacity)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_shrink(utest_fixture->buffer, 0, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 8);
}

UTEST_F(vmnl_net_buffer_ring_shrink, fails_without_memory)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    allocator_limit(0);
    EXPECT_FALSE(vmnl_net_buffer_shrink(utest_fixture->buffer, 4, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 8);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "abc"));
}
