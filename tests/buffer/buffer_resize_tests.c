#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_resize, resizable_types, 16)

UTEST_I(vmnl_net_buffer_resize, grows_capacity, NRESIZABLE_TYPES)
{
    const size_t capacity = vmnl_net_buffer_capacity(utest_fixture->buffer);
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, capacity + 1, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_GE(vmnl_net_buffer_capacity(utest_fixture->buffer), capacity + 1);
}

UTEST_I(vmnl_net_buffer_resize, keeps_stored_bytes, NRESIZABLE_TYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 1, NULL));
    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 4 * VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "ello"));
}

UTEST_I(vmnl_net_buffer_resize, ignores_current_capacity, NRESIZABLE_TYPES)
{
    const size_t capacity = vmnl_net_buffer_capacity(utest_fixture->buffer);
    VmnlNetError error = UINT32_MAX;

    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, capacity, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), capacity);
}

UTEST_I(vmnl_net_buffer_resize, fails_without_memory, NRESIZABLE_TYPES)
{
    const size_t capacity = vmnl_net_buffer_capacity(utest_fixture->buffer);
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    allocator_limit(0);
    EXPECT_FALSE(vmnl_net_buffer_resize(utest_fixture->buffer, 4 * VMNL_NET_BUFFER_CHUNK_SIZE, &error));
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), capacity);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "hello"));
}

UTEST_I(vmnl_net_buffer_resize, accepts_null_error, NRESIZABLE_TYPES)
{
    EXPECT_TRUE(vmnl_net_buffer_resize(utest_fixture->buffer, 4 * VMNL_NET_BUFFER_CHUNK_SIZE, NULL));
}
