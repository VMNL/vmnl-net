#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_chunks_shrink, VMNL_NET_BUFFER_CHUNKS, 4 * VMNL_NET_BUFFER_CHUNK_SIZE)

UTEST_F(vmnl_net_buffer_chunks_shrink, rounds_capacity_up_to_whole_chunks)
{
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 1, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 2 * VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_shrink, keeps_stored_bytes_in_place)
{
    const uint8_t *first;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    first = data_start(utest_fixture->buffer);
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
    EXPECT_EQ(data_start(utest_fixture->buffer), first);
}

UTEST_F(vmnl_net_buffer_chunks_shrink, keeps_chunks_holding_stored_bytes)
{
    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE - 2, NULL));
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 7, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 2 * VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "xxabcde"));
}

UTEST_F(vmnl_net_buffer_chunks_shrink, frees_every_chunk_with_zero_capacity)
{
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 0, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 0);
}

UTEST_F(vmnl_net_buffer_chunks_shrink, allocates_again_after_freeing_every_chunk)
{
    ASSERT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 0, NULL));
    EXPECT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}
