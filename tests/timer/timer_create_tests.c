#include "timer_tests.h"

#include "utest.h"

static void *bad_allocator(size_t size)
{
    (void) size;
    return NULL;
}

TIMER_FIXTURE(vmnl_net_timer_create)

UTEST(vmnl_net_timer_create, creates_inactive_timer)
{
    VmnlNetError error = UINT32_MAX;
    struct Calls calls = {0};
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, &error);

    ASSERT_NE(timer, NULL);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_FALSE(vmnl_net_timer_is_active(timer));
    vmnl_net_timer_destroy(timer);
}

UTEST(vmnl_net_timer_create, creates_one_shot_timer)
{
    struct Calls calls = {0};
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, NULL);

    ASSERT_NE(timer, NULL);
    EXPECT_EQ(vmnl_net_timer_interval(timer), UINT64_C(0));
    vmnl_net_timer_destroy(timer);
}

UTEST(vmnl_net_timer_create, fails_without_memory)
{
    VmnlNetError error = UINT32_MAX;
    struct Calls calls = {0};
    VmnlNetTimer *timer;

    vmnl_net_allocator_set(bad_allocator, realloc, free);
    timer = vmnl_net_timer_create(count_calls, &calls, &error);
    vmnl_net_allocator_set(malloc, realloc, free);
    EXPECT_EQ(timer, NULL);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    vmnl_net_timer_destroy(timer);
}

UTEST_F(vmnl_net_timer_create, accepts_null_data)
{
    VmnlNetTimer *timer = vmnl_net_timer_create(count_null_data, NULL, NULL);

    ASSERT_NE(timer, NULL);
    null_data_calls = 0;
    vmnl_net_timer_start(utest_fixture->context, timer, 0);
    vmnl_net_context_advance(utest_fixture->context, START_TIME);
    EXPECT_EQ(null_data_calls, 1);
    vmnl_net_timer_destroy(timer);
}

UTEST(vmnl_net_timer_create, accepts_null_error)
{
    struct Calls calls = {0};
    VmnlNetTimer *timer = vmnl_net_timer_create(count_calls, &calls, NULL);

    EXPECT_NE(timer, NULL);
    vmnl_net_timer_destroy(timer);
}
