#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_dynamic_resize, VMNL_NET_BUFFER_DYNAMIC, 8)

UTEST_F(vmnl_net_buffer_dynamic_resize, grows_to_exact_capacity)
{
    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 9, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 9);
}
