#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_shrink, resizable_types, 4 * VMNL_NET_BUFFER_CHUNK_SIZE)

UTEST_I(vmnl_net_buffer_shrink, reduces_capacity, NRESIZABLE_TYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_I(vmnl_net_buffer_shrink, keeps_stored_bytes, NRESIZABLE_TYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 1, NULL));
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "ello"));
}

UTEST_I(vmnl_net_buffer_shrink, ignores_current_capacity, NRESIZABLE_TYPES)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, 4 * VMNL_NET_BUFFER_CHUNK_SIZE, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 4 * VMNL_NET_BUFFER_CHUNK_SIZE);
}

UTEST_I(vmnl_net_buffer_shrink, rejects_capacity_below_size, NRESIZABLE_TYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_FALSE(vmnl_net_buffer_shrink(utest_fixture->buffer, 4, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), 4 * VMNL_NET_BUFFER_CHUNK_SIZE);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_shrink, accepts_null_error, NRESIZABLE_TYPES)
{
    EXPECT_TRUE(vmnl_net_buffer_shrink(utest_fixture->buffer, VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
}
