#include "buffer_tests.h"

UTEST(vmnl_net_buffer_count_views_pair, stays_in_first_span_when_bytes_fit)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_pair(10, 5, 8), (size_t) 1);
}

UTEST(vmnl_net_buffer_count_views_pair, stays_in_first_span_when_bytes_fill_it)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_pair(10, 10, 8), (size_t) 2);
}

UTEST(vmnl_net_buffer_count_views_pair, adds_second_span_when_bytes_overflow)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_pair(5, 12, 8), (size_t) 2);
}

UTEST(vmnl_net_buffer_count_views_pair, splits_both_spans_above_max)
{
    EXPECT_EQ(vmnl_net_buffer_count_views_pair(12, 30, 8), (size_t) 5);
}
