#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_ring_prepare, VMNL_NET_BUFFER_RING, 8)

UTEST_F(vmnl_net_buffer_ring_prepare, prepares_one_view_from_start)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 2, views, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].size, (size_t) 5);
}

UTEST_F(vmnl_net_buffer_ring_prepare, prepares_two_views_when_free_space_wraps)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[0].data, data_start(utest_fixture->buffer) + 2);
    EXPECT_EQ(views[0].size, (size_t) 2);
    EXPECT_EQ(views[1].data, data_start(utest_fixture->buffer) - 4);
    EXPECT_EQ(views[1].size, (size_t) 4);
}

UTEST_F(vmnl_net_buffer_ring_prepare, prepares_one_view_before_wrapped_bytes)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghij"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 3, &view, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(view.data, data_start(utest_fixture->buffer) - 3);
    EXPECT_EQ(view.size, (size_t) 3);
}

UTEST_F(vmnl_net_buffer_ring_prepare, writes_around_end_of_block)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    EXPECT_TRUE(append(utest_fixture->buffer, "ghij"));
    EXPECT_TRUE(append(utest_fixture->buffer, "klm"));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "fghijklm"));
}

UTEST_F(vmnl_net_buffer_ring_prepare, prepares_one_view_holding_requested_bytes)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 2, &view, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(view.size, (size_t) 2);
}

UTEST_F(vmnl_net_buffer_ring_prepare, prepares_one_view_from_start_when_stored_bytes_reach_end_of_block)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "gh"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 4, views, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].data, data_start(utest_fixture->buffer) - 4);
    EXPECT_EQ(views[0].size, (size_t) 4);
}

UTEST_F(vmnl_net_buffer_ring_prepare, rejects_one_view_when_requested_bytes_wrap)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 3, &view, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_F(vmnl_net_buffer_ring_prepare, fails_when_full)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdefgh"));
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, views, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_ENOBUFS);
}

UTEST_F(vmnl_net_buffer_ring_prepare, fails_with_more_than_free_space)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 6, views, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_ENOBUFS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "abc"));
}
