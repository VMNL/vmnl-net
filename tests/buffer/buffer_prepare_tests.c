#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_prepare, types, 16)

UTEST_I(vmnl_net_buffer_prepare, prepares_requested_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_GE(views_size(views, nviews), (size_t) 5);
}

UTEST_I(vmnl_net_buffer_prepare, describes_whole_free_space, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, views, &nviews, NULL));
    EXPECT_EQ(views_size(views, nviews), vmnl_net_buffer_capacity(utest_fixture->buffer) - 5);
}

UTEST_I(vmnl_net_buffer_prepare, prepares_after_stored_bytes, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_EQ(views[0].data, data_start(utest_fixture->buffer) + 5);
}

UTEST_I(vmnl_net_buffer_prepare, keeps_stored_bytes, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_prepare, describes_nothing_with_zero_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 0, views, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(nviews, (size_t) 0);
}

UTEST_I(vmnl_net_buffer_prepare, rejects_zero_views, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView view;
    size_t nviews = 0;

    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, &view, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_buffer_prepare, accepts_null_error, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
}
