#include "../buffer_tests.h"

#include "utest.h"

UTEST(vmnl_net_buffer_chunks_create, rounds_capacity_up_to_whole_chunks)
{
    VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_CHUNKS, VMNL_NET_BUFFER_CHUNK_SIZE + 1, NULL);

    ASSERT_NE(buffer, NULL);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), 2 * VMNL_NET_BUFFER_CHUNK_SIZE);
    vmnl_net_buffer_destroy(buffer);
}

UTEST(vmnl_net_buffer_chunks_create, allocates_no_chunk_with_zero_capacity)
{
    VmnlNetBuffer *buffer;

    allocator_limit(1);
    buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_CHUNKS, 0, NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    ASSERT_NE(buffer, NULL);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), (size_t) 0);
    vmnl_net_buffer_destroy(buffer);
}

UTEST(vmnl_net_buffer_chunks_create, fails_without_memory_for_last_chunk)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBuffer *buffer;

    allocator_limit(3);
    buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_CHUNKS, 3 * VMNL_NET_BUFFER_CHUNK_SIZE, &error);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(buffer, NULL);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(live_allocations, (size_t) 0);
}
