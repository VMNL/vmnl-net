#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_ring_resize, VMNL_NET_BUFFER_RING, 8)

UTEST_F(vmnl_net_buffer_ring_resize, grows_to_exact_capacity)
{
    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 9, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 9);
}

UTEST_F(vmnl_net_buffer_ring_resize, unwraps_stored_bytes)
{
    VmnlNetBufferView views[2];
    size_t nviews = 2;

    ASSERT_TRUE(append(utest_fixture->buffer, "abcdef"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 4, NULL));
    ASSERT_TRUE(append(utest_fixture->buffer, "ghijk"));
    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 16, NULL));
    EXPECT_EQ(vmnl_net_buffer_data(utest_fixture->buffer, 0, views, &nviews), (size_t) 7);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "efghijk"));
}
