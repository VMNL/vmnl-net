#include <vmnl/net/context.h>
#include <vmnl/net/allocator.h>

#include "timer/timer_tests.h"

#include "utest.h"

static size_t deallocations;

static void deallocator(void *pointer)
{
    deallocations++;
    free(pointer);
}

UTEST(vmnl_net_context_destroy, frees_context)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);

    ASSERT_NE(context, NULL);
    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    vmnl_net_context_destroy(context);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ((int) deallocations, 1);
}

UTEST(vmnl_net_context_destroy, stops_active_timers)
{
    struct Calls calls = {0};
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, NULL);

    ASSERT_NE(context, NULL);
    ASSERT_NE(timer, NULL);
    vmnl_net_timer_start(context, timer, TIMEOUT);
    vmnl_net_context_destroy(context);
    EXPECT_FALSE(vmnl_net_timer_is_active(timer));
    vmnl_net_timer_destroy(timer);
}

UTEST(vmnl_net_context_destroy, keeps_stopped_timers_usable)
{
    struct Calls calls = {0};
    VmnlNetContext *contexts[2] = {vmnl_net_context_create(NULL), vmnl_net_context_create(NULL)};
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, NULL);

    ASSERT_NE(contexts[0], NULL);
    ASSERT_NE(contexts[1], NULL);
    ASSERT_NE(timer, NULL);
    vmnl_net_timer_start(contexts[0], timer, TIMEOUT);
    vmnl_net_context_destroy(contexts[0]);
    vmnl_net_timer_start(contexts[1], timer, TIMEOUT);
    EXPECT_TRUE(vmnl_net_timer_is_active(timer));
    vmnl_net_timer_destroy(timer);
    vmnl_net_context_destroy(contexts[1]);
}

UTEST(vmnl_net_context_destroy, ignores_null_context)
{
    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    vmnl_net_context_destroy(NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ((int) deallocations, 0);
}
