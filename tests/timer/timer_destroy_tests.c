#include "timer_tests.h"

#include "utest.h"

static int deallocations;

static void deallocator(void *pointer)
{
    deallocations++;
    free(pointer);
}

static void destroy_itself(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    count_calls(timer, lateness, data);
    vmnl_net_timer_destroy(timer);
}

TIMER_FIXTURE(vmnl_net_timer_destroy)

UTEST_F(vmnl_net_timer_destroy, frees_timer)
{
    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    vmnl_net_timer_destroy(utest_fixture->timer);
    utest_fixture->timer = NULL;
    EXPECT_EQ(deallocations, 1);
}

UTEST_F(vmnl_net_timer_destroy, removes_active_timer)
{
    uint64_t remaining;

    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_destroy(utest_fixture->timer);
    utest_fixture->timer = NULL;
    EXPECT_FALSE(vmnl_net_context_remaining(utest_fixture->context, START_TIME, &remaining));
}

UTEST_F(vmnl_net_timer_destroy, frees_timer_in_one_shot_callback)
{
    vmnl_net_timer_set_callback(utest_fixture->timer, destroy_itself, &utest_fixture->calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    utest_fixture->timer = NULL;
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(deallocations, 1);
}

UTEST_F(vmnl_net_timer_destroy, frees_timer_in_periodic_callback)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_callback(utest_fixture->timer, destroy_itself, &utest_fixture->calls);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    utest_fixture->timer = NULL;
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(deallocations, 1);
}

UTEST(vmnl_net_timer_destroy, ignores_null_timer)
{
    vmnl_net_allocator_set(malloc, realloc, deallocator);
    deallocations = 0;
    vmnl_net_timer_destroy(NULL);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(deallocations, 0);
}
