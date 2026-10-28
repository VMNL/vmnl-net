#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_consume, types, 16)

UTEST_I(vmnl_net_buffer_consume, drops_first_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "llo"));
}

UTEST_I(vmnl_net_buffer_consume, drops_every_byte, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, ""));
}

UTEST_I(vmnl_net_buffer_consume, frees_space_of_dropped_bytes, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(commit(utest_fixture->buffer, vmnl_net_buffer_capacity(utest_fixture->buffer)));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, vmnl_net_buffer_capacity(utest_fixture->buffer), NULL));
    allocator_limit(0);
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, vmnl_net_buffer_capacity(utest_fixture->buffer), views, &nviews, NULL));
}

UTEST_I(vmnl_net_buffer_consume, rewinds_when_emptied, NTYPES)
{
    const uint8_t *first;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    first = data_start(utest_fixture->buffer);
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_EQ(data_start(utest_fixture->buffer), first);
}

UTEST_I(vmnl_net_buffer_consume, ignores_zero_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 0, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_consume, rejects_more_than_size, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_FALSE(vmnl_net_buffer_consume(utest_fixture->buffer, 6, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_consume, accepts_null_error, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, NULL));
}
