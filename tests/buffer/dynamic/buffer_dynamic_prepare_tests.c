#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_dynamic_prepare, VMNL_NET_BUFFER_DYNAMIC, 0)

UTEST_F(vmnl_net_buffer_dynamic_prepare, grows_to_256_bytes_at_least)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, &view, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 256);
    EXPECT_EQ(view.size, (size_t) 256);
}

UTEST_F(vmnl_net_buffer_dynamic_prepare, grows_to_one_and_a_half_capacity)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(commit(utest_fixture->buffer, 256));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, &view, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 384);
}

UTEST_F(vmnl_net_buffer_dynamic_prepare, grows_to_needed_bytes)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(commit(utest_fixture->buffer, 256));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1000, &view, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 1256);
}

UTEST_F(vmnl_net_buffer_dynamic_prepare, keeps_stored_bytes_when_growing)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 300, &view, &nviews, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_F(vmnl_net_buffer_dynamic_prepare, moves_stored_bytes_to_front_before_growing)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(commit(utest_fixture->buffer, 256));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 250, NULL));
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 200, &view, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 256);
    EXPECT_EQ(view.size, (size_t) 250);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "xxxxxx"));
}

UTEST_F(vmnl_net_buffer_dynamic_prepare, keeps_capacity_with_exactly_free_space)
{
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(commit(utest_fixture->buffer, 200));
    allocator_limit(0);
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 56, &view, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 256);
    EXPECT_EQ(view.size, (size_t) 56);
}

UTEST_F(vmnl_net_buffer_dynamic_prepare, fails_without_memory)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(commit(utest_fixture->buffer, 256));
    allocator_limit(0);
    EXPECT_FALSE(vmnl_net_buffer_prepare(utest_fixture->buffer, 1, &view, &nviews, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 256);
}
