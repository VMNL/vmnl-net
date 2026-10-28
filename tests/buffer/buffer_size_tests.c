#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_size, types, 16)

UTEST_I(vmnl_net_buffer_size, counts_committed_bytes, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_size(utest_fixture->buffer), (size_t) 5);
    ASSERT_TRUE(append(utest_fixture->buffer, "abc"));
    EXPECT_EQ(vmnl_net_buffer_size(utest_fixture->buffer), (size_t) 8);
}

UTEST_I(vmnl_net_buffer_size, uncounts_consumed_bytes, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, NULL));
    EXPECT_EQ(vmnl_net_buffer_size(utest_fixture->buffer), (size_t) 3);
}

UTEST_I(vmnl_net_buffer_size, uncounts_prepared_bytes, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 5, views, &nviews, NULL));
    EXPECT_EQ(vmnl_net_buffer_size(utest_fixture->buffer), (size_t) 0);
}

UTEST_I(vmnl_net_buffer_size, returns_zero_when_new, NTYPES)
{
    EXPECT_EQ(vmnl_net_buffer_size(utest_fixture->buffer), (size_t) 0);
}
