#include <vmnl/net/context.h>
#include <vmnl/net/allocator.h>

#include "utest.h"

static void *bad_allocator(size_t size)
{
    (void) size;
    return NULL;
}

UTEST(vmnl_net_context_create, creates_context)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create(&error);

    EXPECT_NE(context, NULL);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    vmnl_net_context_destroy(context);
}

UTEST(vmnl_net_context_create, creates_context_without_error_parameter)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);

    EXPECT_NE(context, NULL);
    vmnl_net_context_destroy(context);
}

UTEST(vmnl_net_context_create, fails_with_memory_allocation_error)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context;

    vmnl_net_allocator_set(bad_allocator, realloc, free);
    context = vmnl_net_context_create(&error);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(context, NULL);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    vmnl_net_context_destroy(context);
}
