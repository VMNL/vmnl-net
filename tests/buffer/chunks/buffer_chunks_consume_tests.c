#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_chunks_consume, VMNL_NET_BUFFER_CHUNKS, 0)

UTEST_F(vmnl_net_buffer_chunks_consume, drops_bytes_across_chunks)
{
    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 2, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "cde"));
}

UTEST_F(vmnl_net_buffer_chunks_consume, recycles_emptied_chunks)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 5));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
    allocator_limit(0);
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 2 * VMNL_NET_BUFFER_CHUNK_SIZE - 5, views, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 2 * VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_consume, keeps_chunk_with_one_byte_left)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE - 1, NULL));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 6);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "xabcde"));
}
