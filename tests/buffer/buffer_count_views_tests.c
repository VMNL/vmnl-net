#include "buffer_tests.h"

UTEST(vmnl_net_buffer_count_views, returns_one_when_bytes_fit)
{
    EXPECT_EQ(vmnl_net_buffer_count_views(5, 8), (size_t) 1);
}

UTEST(vmnl_net_buffer_count_views, returns_one_with_max_bytes)
{
    EXPECT_EQ(vmnl_net_buffer_count_views(8, 8), (size_t) 1);
}

UTEST(vmnl_net_buffer_count_views, returns_quotient_with_multiple_of_max)
{
    EXPECT_EQ(vmnl_net_buffer_count_views(16, 8), (size_t) 2);
}

UTEST(vmnl_net_buffer_count_views, rounds_up_with_remainder)
{
    EXPECT_EQ(vmnl_net_buffer_count_views(17, 8), (size_t) 3);
}
