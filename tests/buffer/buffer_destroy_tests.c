#include "buffer_tests.h"

#include "utest.h"

struct vmnl_net_buffer_destroy {
    VmnlNetBufferType type;
};

UTEST_I_SETUP(vmnl_net_buffer_destroy)
{
    (void) utest_result;
    utest_fixture->type = types[utest_index];
}

UTEST_I_TEARDOWN(vmnl_net_buffer_destroy)
{
    (void) utest_result;
    (void) utest_fixture;
    (void) utest_index;
    vmnl_net_allocator_set(malloc, realloc, free);
}

UTEST_I(vmnl_net_buffer_destroy, frees_every_allocation, NTYPES)
{
    VmnlNetBuffer *buffer;

    allocator_limit(SIZE_MAX);
    buffer = vmnl_net_buffer_create(utest_fixture->type, 2 * VMNL_NET_BUFFER_CHUNK_SIZE, NULL);
    ASSERT_NE(buffer, NULL);
    ASSERT_TRUE(append(buffer, "hello"));
    vmnl_net_buffer_destroy(buffer);
    EXPECT_EQ(live_allocations, (size_t) 0);
}

UTEST(vmnl_net_buffer_destroy, ignores_null_buffer)
{
    allocator_limit(0);
    vmnl_net_buffer_destroy(NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(deallocations, (size_t) 0);
}
