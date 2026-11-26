#include "buffer_tests.h"

UTEST(vmnl_net_buffer_describe_pair, fills_one_view_per_span_when_bytes_fit)
{
    uint8_t first[20];
    uint8_t second[20];
    VmnlNetBufferView views[4];
    size_t nviews = 4;

    EXPECT_EQ(vmnl_net_buffer_describe_pair((struct VmnlNetBufferSpan) {.data = first, .size = 5}, (struct VmnlNetBufferSpan) {.data = second, .size = 6}, 8, views, &nviews), (size_t) 11);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[0].data, first);
    EXPECT_EQ(views[0].size, (VmnlNetBufferViewSize) 5);
    EXPECT_EQ(views[1].data, second);
    EXPECT_EQ(views[1].size, (VmnlNetBufferViewSize) 6);
}

UTEST(vmnl_net_buffer_describe_pair, splits_both_spans_above_max)
{
    uint8_t first[20];
    uint8_t second[20];
    VmnlNetBufferView views[4];
    size_t nviews = 4;

    EXPECT_EQ(vmnl_net_buffer_describe_pair((struct VmnlNetBufferSpan) {.data = first, .size = 12}, (struct VmnlNetBufferSpan) {.data = second, .size = 10}, 8, views, &nviews), (size_t) 22);
    EXPECT_EQ(nviews, (size_t) 4);
    EXPECT_EQ(views[0].data, first);
    EXPECT_EQ(views[0].size, (VmnlNetBufferViewSize) 8);
    EXPECT_EQ(views[1].data, first + 8);
    EXPECT_EQ(views[1].size, (VmnlNetBufferViewSize) 4);
    EXPECT_EQ(views[2].data, second);
    EXPECT_EQ(views[2].size, (VmnlNetBufferViewSize) 8);
    EXPECT_EQ(views[3].data, second + 8);
    EXPECT_EQ(views[3].size, (VmnlNetBufferViewSize) 2);
}

UTEST(vmnl_net_buffer_describe_pair, stops_when_first_span_fills_views)
{
    uint8_t first[20];
    uint8_t second[20];
    VmnlNetBufferView views[4];
    size_t nviews = 2;

    EXPECT_EQ(vmnl_net_buffer_describe_pair((struct VmnlNetBufferSpan) {.data = first, .size = 20}, (struct VmnlNetBufferSpan) {.data = second, .size = 6}, 8, views, &nviews), (size_t) 16);
    EXPECT_EQ(nviews, (size_t) 2);
    EXPECT_EQ(views[1].data, first + 8);
    EXPECT_EQ(views[1].size, (VmnlNetBufferViewSize) 8);
}

UTEST(vmnl_net_buffer_describe_pair, stops_when_second_span_fills_views)
{
    uint8_t first[20];
    uint8_t second[20];
    VmnlNetBufferView views[4];
    size_t nviews = 3;

    EXPECT_EQ(vmnl_net_buffer_describe_pair((struct VmnlNetBufferSpan) {.data = first, .size = 8}, (struct VmnlNetBufferSpan) {.data = second, .size = 20}, 8, views, &nviews), (size_t) 24);
    EXPECT_EQ(nviews, (size_t) 3);
    EXPECT_EQ(views[2].data, second + 8);
    EXPECT_EQ(views[2].size, (VmnlNetBufferViewSize) 8);
}

UTEST(vmnl_net_buffer_describe_pair, skips_empty_second_span)
{
    uint8_t first[20];
    uint8_t second[20];
    VmnlNetBufferView views[4];
    size_t nviews = 4;

    EXPECT_EQ(vmnl_net_buffer_describe_pair((struct VmnlNetBufferSpan) {.data = first, .size = 5}, (struct VmnlNetBufferSpan) {.data = second, .size = 0}, 8, views, &nviews), (size_t) 5);
    EXPECT_EQ(nviews, (size_t) 1);
    EXPECT_EQ(views[0].data, first);
    EXPECT_EQ(views[0].size, (VmnlNetBufferViewSize) 5);
}
