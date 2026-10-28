#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_flat_prepare, VMNL_NET_BUFFER_FLAT, 8)

UTEST_F(vmnl_net_buffer_flat_prepare, prepares_one_view)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].size, (size_t) 5);
}

UTEST_F(vmnl_net_buffer_flat_prepare, keeps_stored_bytes_in_place_with_room_after_them)
{
    VmnlNetBufferView view;
    size_t nviews = 1;
    const uint8_t *first;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcd"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, NULL));
    first = data_start(utest_fixture->buffer);
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 4, &view, &nviews, NULL));
    EXPECT_EQ(data_start(utest_fixture->buffer), first);
    EXPECT_EQ(view.size, (size_t) 4);
}

UTEST_F(vmnl_net_buffer_flat_prepare, moves_stored_bytes_to_front_without_room_after_them)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcd"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, NULL));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, &view, &nviews, NULL));
    EXPECT_EQ(view.size, (size_t) 6);
    EXPECT_EQ(view.data, data_start(utest_fixture->buffer) + 2);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "cd"));
}

UTEST_F(vmnl_net_buffer_flat_prepare, fails_when_full)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdefgh"));
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, &view, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_ENOBUFS);
}

UTEST_F(vmnl_net_buffer_flat_prepare, fails_with_more_than_free_space)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 6, &view, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_ENOBUFS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "abc"));
}
