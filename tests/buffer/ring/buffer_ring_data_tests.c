#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_ring_data, VMNL_NET_BUFFER_RING, 8)

UTEST_F(vmnl_net_buffer_ring_data, describes_two_views_when_wrapped)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 7);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[0].size, (size_t) 4);
    EXPECT_EQ(memcmp(views[0].data, "efgh", 4), 0);
    EXPECT_EQ(views[1].size, (size_t) 3);
    EXPECT_EQ(memcmp(views[1].data, "ijk", 3), 0);
}

UTEST_F(vmnl_net_buffer_ring_data, describes_one_view_past_end_of_block)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 5, views, &nviews), (size_t) 2);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(views[0].data, "jk", 2), 0);
}

UTEST_F(vmnl_net_buffer_ring_data, describes_bytes_before_end_of_block_with_one_view)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 1, &view, &nviews), (size_t) 3);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(view.data, "fgh", 3), 0);
}

UTEST_F(vmnl_net_buffer_ring_data, describes_one_view_when_stored_bytes_reach_end_of_block)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "gh"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 4);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(views[0].data, "efgh", 4), 0);
}

UTEST_F(vmnl_net_buffer_ring_data, describes_one_view_with_offset_at_end_of_block)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 4, views, &nviews), (size_t) 3);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(views[0].data, "ijk", 3), 0);
}

UTEST_F(vmnl_net_buffer_ring_data, returns_zero_without_views_when_wrapped)
{
    VmnlNetBufferView view;
    size_t nviews = 0;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, &view, &nviews), (size_t) 0);
    EXPECT_EQ(nviews, (size_t) 0);
}
