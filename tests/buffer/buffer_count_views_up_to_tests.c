#include "buffer_tests.h"

#include "utest.h"

UTEST(vmnl_net_buffer_count_views_up_to, counts_one_view_when_bytes_fit)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_up_to(5, 8), (size_t) 1);
}

UTEST(vmnl_net_buffer_count_views_up_to, counts_one_view_at_the_limit)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_up_to(8, 8), (size_t) 1);
}

UTEST(vmnl_net_buffer_count_views_up_to, counts_a_multiple_of_the_limit)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_up_to(16, 8), (size_t) 2);
}

UTEST(vmnl_net_buffer_count_views_up_to, counts_a_view_for_the_rest)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_up_to(17, 8), (size_t) 3);
}
