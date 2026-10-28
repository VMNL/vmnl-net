#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_clear, types, 16)

UTEST_I(vmnl_net_buffer_clear, drops_stored_bytes, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    vmnl_net_buffer_clear(utest_fixture->buffer);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, ""));
}

UTEST_I(vmnl_net_buffer_clear, keeps_capacity, NTYPES)
{
    const size_t capacity = vmnl_net_buffer_capacity(utest_fixture->buffer);

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    vmnl_net_buffer_clear(utest_fixture->buffer);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), capacity);
}

UTEST_I(vmnl_net_buffer_clear, frees_whole_capacity, NTYPES)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, NULL));
    vmnl_net_buffer_clear(utest_fixture->buffer);
    allocator_limit(0);
    EXPECT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, vmnl_net_buffer_capacity(utest_fixture->buffer), views, &nviews, NULL));
}

UTEST_I(vmnl_net_buffer_clear, ignores_empty_buffer, NTYPES)
{
    vmnl_net_buffer_clear(utest_fixture->buffer);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, ""));
}
