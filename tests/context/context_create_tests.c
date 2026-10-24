#include <vmnl/net/context.h>
#include <vmnl/net/allocator.h>

#include "utest.h"

static void *failing_allocator(size_t size)
{
    (void) size;
    return NULL;
}

UTEST(vmnl_net_context_create, success)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create(&error);

    ASSERT_NE(context, NULL);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    vmnl_net_context_destroy(context);
}

UTEST(vmnl_net_context_create, allocation_failure)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context;

    vmnl_net_allocator_set(failing_allocator, realloc, free);
    context = vmnl_net_context_create(&error);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(context, NULL);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
}

UTEST(vmnl_net_context_create, null_error)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);

    EXPECT_NE(context, NULL);
    vmnl_net_context_destroy(context);
}
