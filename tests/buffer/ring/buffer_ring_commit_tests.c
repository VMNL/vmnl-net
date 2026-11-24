#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_ring_commit, VMNL_NET_BUFFER_RING, 8)

UTEST_F(vmnl_net_buffer_ring_commit, stores_bytes_around_end_of_block)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 6, views, &nviews, NULL));
    ASSERT_EQ(nviews, (size_t) 2);
    memcpy(views[0].data, "gh", 2);
    memcpy(views[1].data, "ijkl", 4);
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, 6, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "efghijkl"));
}

UTEST_F(vmnl_net_buffer_ring_commit, rejects_more_than_free_space_with_stored_bytes)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_FALSE(vmnl_net_buffer_commit(utest_fixture->buffer, 6, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "abc"));
}
