#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_commit, types, 16)

UTEST_I(vmnl_net_buffer_commit, stores_written_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    memcpy(views[0].data, "hello", 5);
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, 5, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_commit, stores_first_prepared_bytes, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    memcpy(views[0].data, "hello", 5);
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, 3, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hel"));
}

UTEST_I(vmnl_net_buffer_commit, appends_to_stored_bytes, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    ASSERT_TRUE(append(utest_fixture->buffer, " you"));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello you"));
}

UTEST_I(vmnl_net_buffer_commit, fills_whole_capacity, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, vmnl_net_buffer_capacity(utest_fixture->buffer), views, &nviews, NULL));
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, vmnl_net_buffer_capacity(utest_fixture->buffer), NULL));
    EXPECT_EQ(vmnl_net_buffer_size(utest_fixture->buffer), vmnl_net_buffer_capacity(utest_fixture->buffer));
}

UTEST_I(vmnl_net_buffer_commit, ignores_zero_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, 0, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_commit, rejects_more_than_free_space, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_FALSE(vmnl_net_buffer_commit(utest_fixture->buffer, vmnl_net_buffer_capacity(utest_fixture->buffer) + 1, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, ""));
}

UTEST_I(vmnl_net_buffer_commit, accepts_null_error, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, 5, NULL));
}
