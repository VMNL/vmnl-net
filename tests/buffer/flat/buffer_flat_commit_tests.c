#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_flat_commit, VMNL_NET_BUFFER_FLAT, 8)

UTEST_F(vmnl_net_buffer_flat_commit, rejects_consumed_space_before_stored_bytes)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetBufferView view;
    size_t nviews = 1;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(vmnl_net_buffer_prepare(utest_fixture->buffer, 2, &view, &nviews, NULL));
    EXPECT_FALSE(vmnl_net_buffer_commit(utest_fixture->buffer, 3, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "ef"));
}
