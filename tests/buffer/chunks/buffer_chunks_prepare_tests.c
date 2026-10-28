#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_chunks_prepare, VMNL_NET_BUFFER_CHUNKS, 0)

UTEST_F(vmnl_net_buffer_chunks_prepare, adds_chunks_for_requested_bytes)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 1, views, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 2 * VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[0].size, VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(views[1].size, VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, prepares_one_view_per_chunk)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, views, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[0].data, data_start(utest_fixture->buffer) + 3);
    EXPECT_EQ(views[0].size, VMNL_NET_BUFFER_CHUNK_SIZE - 3);
    EXPECT_EQ(views[1].size, VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, keeps_stored_bytes_in_place_when_growing)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;
    const uint8_t *first;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    first = data_start(utest_fixture->buffer);
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 2 * VMNL_NET_BUFFER_CHUNK_SIZE, views, &nviews, NULL));
    EXPECT_EQ(data_start(utest_fixture->buffer), first);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_F(vmnl_net_buffer_chunks_prepare, skips_consumed_bytes_of_first_chunk)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 10, NULL));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, views, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 2 * VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].size, VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, prepares_one_view_with_exactly_one_chunk)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, views, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].size, VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, prepares_next_chunk_when_last_one_is_full)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, views, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].size, VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, keeps_capacity_with_exactly_free_space)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcde"));
    allocator_limit(0);
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE - 5, views, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_EQ(nviews, (size_t) 1);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, prepares_first_chunks_with_fewer_views)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    ASSERT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 3 * VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, &view, &nviews, NULL));
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(view.size, VMNL_NET_BUFFER_CHUNK_SIZE - 3);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, rejects_too_few_views)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView view;
    size_t nviews = 1;

    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE + 1, &view, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, fails_without_memory)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    allocator_limit(0);
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, views, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 0);
}

UTEST_F(vmnl_net_buffer_chunks_prepare, fails_without_memory_with_overflowing_size)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, SIZE_MAX, views, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), VMNL_NET_BUFFER_CHUNK_SIZE);
}
