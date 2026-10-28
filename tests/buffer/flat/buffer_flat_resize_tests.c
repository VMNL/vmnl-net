#include "../buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_F(vmnl_net_buffer_flat_resize, VMNL_NET_BUFFER_FLAT, 8)

UTEST_F(vmnl_net_buffer_flat_resize, fails_as_unsupported)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_buffer_resize(utest_fixture->buffer, 16, &error));
    EXPECT_EQ(error, VMNL_NET_ENOTSUP);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 8);
}
