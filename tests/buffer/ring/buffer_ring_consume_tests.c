#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_ring_consume, VMNL_NET_BUFFER_RING, 8)

UTEST_F(vmnl_net_buffer_ring_consume, drops_bytes_around_end_of_block)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "jk"));
}

UTEST_F(vmnl_net_buffer_ring_consume, drops_bytes_up_to_end_of_block)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "ijk"));
}
