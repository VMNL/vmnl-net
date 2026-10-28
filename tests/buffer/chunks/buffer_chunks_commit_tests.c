#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_chunks_commit, VMNL_NET_BUFFER_CHUNKS, 0)

UTEST_F(vmnl_net_buffer_chunks_commit, stores_bytes_across_chunks)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 5, views, &nviews, NULL));
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 5, NULL));
    nviews = 8;
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), VMNL_NET_BUFFER_CHUNK_SIZE + 5);
    EXPECT_EQ(nviews, (size_t) 2);
}

UTEST_F(vmnl_net_buffer_chunks_commit, continues_in_next_chunk_after_filling_one)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 3, views, &nviews, NULL));
    memcpy(views[0].data, "abc", 3);
    EXPECT_TRUE(vmnl_net_buffer_commit(utest_fixture->buffer, 3, NULL));
    nviews = 8;
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, views, &nviews), (size_t) 3);
    EXPECT_EQ(memcmp(views[0].data, "abc", 3), 0);
}

UTEST_F(vmnl_net_buffer_chunks_commit, rejects_consumed_space_of_first_chunk)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdefghij"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE - 10, views, &nviews, NULL));
    EXPECT_FALSE(vmnl_net_buffer_commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE - 9, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "efghij"));
}
