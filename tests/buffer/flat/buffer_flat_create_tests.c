#include "../buffer_tests.h"

#include "utest.h"

UTEST(vmnl_net_buffer_flat_create, creates_exact_capacity)
{
    VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_FLAT, 1, NULL);

    ASSERT_NE(buffer, NULL);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), (size_t) 1);
    vmnl_net_buffer_destroy(buffer);
}

UTEST(vmnl_net_buffer_flat_create, rejects_zero_capacity)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_buffer_create(VMNL_NET_BUFFER_FLAT, 0, &error), NULL);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
