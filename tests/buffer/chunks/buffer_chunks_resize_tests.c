#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_chunks_resize, VMNL_NET_BUFFER_CHUNKS, VMNL_NET_BUFFER_CHUNK_SIZE)

UTEST_F(vmnl_net_buffer_chunks_resize, rounds_capacity_up_to_whole_chunks)
{
    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 2 * VMNL_NET_BUFFER_CHUNK_SIZE + 1, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 3 * VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_resize, keeps_stored_bytes_in_place)
{
    const uint8_t *first;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    first = data_start(utest_fixture->buffer);
    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 3 * VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
    EXPECT_EQ(data_start(utest_fixture->buffer), first);
}
