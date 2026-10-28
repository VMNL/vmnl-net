#include "../buffer_tests.h"

#include "utest.h"

UTEST(vmnl_net_buffer_dynamic_create, creates_exact_capacity)
{
    VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_DYNAMIC, 13, NULL);

    ASSERT_NE(buffer, NULL);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), (size_t) 13);
    vmnl_net_buffer_destroy(buffer);
}

UTEST(vmnl_net_buffer_dynamic_create, allocates_no_byte_with_zero_capacity)
{
    VmnlNetBuffer *buffer;

    allocator_limit(1);
    buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_DYNAMIC, 0, NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    ASSERT_NE(buffer, NULL);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), (size_t) 0);
    vmnl_net_buffer_destroy(buffer);
}
