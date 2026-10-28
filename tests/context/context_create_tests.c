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

UTEST(vmnl_net_context_create, fails_without_memory)
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

UTEST(vmnl_net_context_create, accepts_null_error)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);

    EXPECT_NE(context, NULL);
    vmnl_net_context_destroy(context);
}
