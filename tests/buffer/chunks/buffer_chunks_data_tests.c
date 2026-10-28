#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_chunks_data, VMNL_NET_BUFFER_CHUNKS, 0)

UTEST_F(vmnl_net_buffer_chunks_data, describes_one_view_per_chunk)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), VMNL_NET_BUFFER_CHUNK_SIZE + 5);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[0].size, VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(memcmp(views[1].data, "abcde", 5), 0);
}

UTEST_F(vmnl_net_buffer_chunks_data, skips_whole_chunks)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 2, views, &nviews), (size_t) 3);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(views[0].data, "cde", 3), 0);
}

UTEST_F(vmnl_net_buffer_chunks_data, describes_first_chunks_with_too_few_views)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(commit(utest_fixture->buffer, 2 * VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), 2 * VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(nviews, (size_t) 2);
}

UTEST_F(vmnl_net_buffer_chunks_data, describes_one_view_with_exactly_one_chunk)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(nviews, (size_t) 1);
}

UTEST_F(vmnl_net_buffer_chunks_data, starts_at_next_chunk_with_offset_of_one_chunk)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, views, &nviews), (size_t) 5);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(memcmp(views[0].data, "abcde", 5), 0);
}

UTEST_F(vmnl_net_buffer_chunks_data, describes_last_byte_of_chunk_with_offset_one_before)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE - 1, views, &nviews), (size_t) 6);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[0].size, (size_t) 1);
    EXPECT_EQ(memcmp(views[1].data, "abcde", 5), 0);
}
