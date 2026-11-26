#include "buffer_tests.h"

#include "utest.h"

UTEST(vmnl_net_buffer_describe_up_to, fills_one_view_when_bytes_fit)
{
    uint8_t bytes[20];
    VmnlNetBufferView views[4];
    size_t nviews = 4;

    EXPECT_EQ(vmnl_net_buffer_describe_up_to(bytes, 5, 8, views, &nviews), (size_t) 5);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].data, bytes);
    EXPECT_EQ(views[0].size, (VmnlNetBufferViewSize) 5);
}

UTEST(vmnl_net_buffer_describe_up_to, fills_one_view_at_the_limit)
{
    uint8_t bytes[20];
    VmnlNetBufferView views[4];
    size_t nviews = 4;

    EXPECT_EQ(vmnl_net_buffer_describe_up_to(bytes, 8, 8, views, &nviews), (size_t) 8);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].data, bytes);
    EXPECT_EQ(views[0].size, (VmnlNetBufferViewSize) 8);
}

UTEST(vmnl_net_buffer_describe_up_to, splits_bytes_past_the_limit)
{
    uint8_t bytes[20];
    VmnlNetBufferView views[4];
    size_t nviews = 4;

    EXPECT_EQ(vmnl_net_buffer_describe_up_to(bytes, 20, 8, views, &nviews), (size_t) 20);
    EXPECT_EQ(nviews, (size_t) 3);
    EXPECT_EQ(views[0].data, bytes);
    EXPECT_EQ(views[0].size, (VmnlNetBufferViewSize) 8);
    EXPECT_EQ(views[1].data, bytes + 8);
    EXPECT_EQ(views[1].size, (VmnlNetBufferViewSize) 8);
    EXPECT_EQ(views[2].data, bytes + 16);
    EXPECT_EQ(views[2].size, (VmnlNetBufferViewSize) 4);
}

UTEST(vmnl_net_buffer_describe_up_to, splits_a_multiple_of_the_limit)
{
    uint8_t bytes[20];
    VmnlNetBufferView views[4];
    size_t nviews = 4;

    EXPECT_EQ(vmnl_net_buffer_describe_up_to(bytes, 16, 8, views, &nviews), (size_t) 16);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[1].data, bytes + 8);
    EXPECT_EQ(views[1].size, (VmnlNetBufferViewSize) 8);
}

UTEST(vmnl_net_buffer_describe_up_to, stops_when_views_run_out)
{
    uint8_t bytes[20];
    VmnlNetBufferView views[4];
    size_t nviews = 2;

    EXPECT_EQ(vmnl_net_buffer_describe_up_to(bytes, 20, 8, views, &nviews), (size_t) 16);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[1].data, bytes + 8);
    EXPECT_EQ(views[1].size, (VmnlNetBufferViewSize) 8);
}
