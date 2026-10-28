#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_data, types, 16)

UTEST_I(vmnl_net_buffer_data, describes_stored_bytes, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 5);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(views[0].data, "hello", 5), 0);
}

UTEST_I(vmnl_net_buffer_data, skips_offset, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 2, views, &nviews), (size_t) 3);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(views[0].data, "llo", 3), 0);
}

UTEST_I(vmnl_net_buffer_data, keeps_stored_bytes, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 5);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_data, describes_last_byte_with_offset_one_before_size, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 4, views, &nviews), (size_t) 1);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].data[0], 'o');
}

UTEST_I(vmnl_net_buffer_data, returns_zero_when_empty, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 0);
    EXPECT_EQ(nviews, (size_t) 0);
}

UTEST_I(vmnl_net_buffer_data, returns_zero_with_offset_at_size, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 5, views, &nviews), (size_t) 0);
    EXPECT_EQ(nviews, (size_t) 0);
}

UTEST_I(vmnl_net_buffer_data, returns_zero_without_views, NTYPES)
{
    VmnlNetBufferView view;
    size_t nviews = 0;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, &view, &nviews), (size_t) 0);
    EXPECT_EQ(nviews, (size_t) 0);
}
