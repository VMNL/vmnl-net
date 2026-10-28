#include "buffer_tests.h"

#include "utest.h"

struct vmnl_net_buffer_create {
    VmnlNetBufferType type;
    VmnlNetBuffer *buffer;
};

UTEST_I_SETUP(vmnl_net_buffer_create)
{
    (void) utest_result;
    utest_fixture->type = types[utest_index];
}

UTEST_I_TEARDOWN(vmnl_net_buffer_create)
{
    (void) utest_result;
    (void) utest_index;
    vmnl_net_allocator_set(malloc, realloc, free);
    vmnl_net_buffer_destroy(utest_fixture->buffer);
}

UTEST_I(vmnl_net_buffer_create, creates_empty_buffer, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    utest_fixture->buffer = vmnl_net_buffer_create(utest_fixture->type, 16, &error);
    ASSERT_NE(utest_fixture->buffer, NULL);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(vmnl_net_buffer_is_empty(utest_fixture->buffer));
    EXPECT_GE(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 16);
}

UTEST(vmnl_net_buffer_create, rejects_unknown_type)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_buffer_create(0, 16, &error), NULL);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    error = UINT32_MAX;
    EXPECT_EQ(vmnl_net_buffer_create(VMNL_NET_BUFFER_CHUNKS + 1, 16, &error), NULL);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST_I(vmnl_net_buffer_create, fails_without_memory, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    allocator_limit(0);
    utest_fixture->buffer = vmnl_net_buffer_create(utest_fixture->type, 16, &error);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(utest_fixture->buffer, NULL);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
}

UTEST_I(vmnl_net_buffer_create, fails_without_memory_for_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    allocator_limit(1);
    utest_fixture->buffer = vmnl_net_buffer_create(utest_fixture->type, 16, &error);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(utest_fixture->buffer, NULL);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    EXPECT_EQ(live_allocations, (size_t) 0);
}

UTEST_I(vmnl_net_buffer_create, accepts_null_error, NTYPES)
{
    utest_fixture->buffer = vmnl_net_buffer_create(utest_fixture->type, 16, NULL);
    EXPECT_NE(utest_fixture->buffer, NULL);
}
