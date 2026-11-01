#include "timer/timer_tests.h"

#include "utest.h"

struct Order {
    int *calls;
    int position;
};

struct Target {
    struct Calls calls;
    VmnlNetContext *context;
    VmnlNetTimer *timer;
    uint64_t timeout;
};

static void record_position(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    struct Order *order = data;

    (void) timer;
    (void) lateness;
    order->position = ++*order->calls;
}

static void start_target(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    struct Target *target = data;

    count_calls(timer, lateness, &target->calls);
    vmnl_net_timer_start(target->context, target->timer, target->timeout);
}

static void stop_target(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    struct Target *target = data;

    count_calls(timer, lateness, &target->calls);
    vmnl_net_timer_stop(target->timer);
}

static void destroy_target(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    struct Target *target = data;

    count_calls(timer, lateness, &target->calls);
    vmnl_net_timer_destroy(target->timer);
}

TIMER_FIXTURE(vmnl_net_context_advance)

UTEST_F(vmnl_net_context_advance, calls_timer_at_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(utest_fixture->calls.count, 1);
}

UTEST_F(vmnl_net_context_advance, passes_lateness_after_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT + 30);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(utest_fixture->calls.lateness, UINT64_C(30));
}

UTEST_F(vmnl_net_context_advance, calls_timers_in_deadline_order)
{
    int calls = 0;
    struct Order orders[3] = {{&calls, 0}, {&calls, 0}, {&calls, 0}};
    VmnlNetTimer *timers[2];

    timers[0] = vmnl_net_timer_create(record_position, &orders[1], NULL);
    timers[1] = vmnl_net_timer_create(record_position, &orders[2], NULL);
    ASSERT_NE(timers[0], NULL);
    ASSERT_NE(timers[1], NULL);
    vmnl_net_timer_set_callback(utest_fixture->timer, record_position, &orders[0]);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, 30);
    vmnl_net_timer_start(utest_fixture->context, timers[0], 10);
    vmnl_net_timer_start(utest_fixture->context, timers[1], 20);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(orders[0].position, 3);
    EXPECT_EQ(orders[1].position, 1);
    EXPECT_EQ(orders[2].position, 2);
    vmnl_net_timer_destroy(timers[0]);
    vmnl_net_timer_destroy(timers[1]);
}

UTEST_F(vmnl_net_context_advance, calls_timers_with_same_deadline_in_start_order)
{
    int calls = 0;
    struct Order orders[2] = {{&calls, 0}, {&calls, 0}};
    VmnlNetTimer *timer = vmnl_net_timer_create(record_position, &orders[1], NULL);

    ASSERT_NE(timer, NULL);
    vmnl_net_timer_set_callback(utest_fixture->timer, record_position, &orders[0]);
    vmnl_net_timer_start(utest_fixture->context, timer, TIMEOUT);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(orders[0].position, 2);
    EXPECT_EQ(orders[1].position, 1);
    vmnl_net_timer_destroy(timer);
}

UTEST_F(vmnl_net_context_advance, calls_periodic_timer_once_after_missed_deadlines)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT / 2);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT + 120);
    EXPECT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(utest_fixture->calls.lateness, UINT64_C(120));
}

UTEST_F(vmnl_net_context_advance, keeps_phase_of_periodic_timer)
{
    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT / 2);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT + 120);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + TIMEOUT + 120), UINT64_C(30));
}

UTEST_F(vmnl_net_context_advance, skips_rearm_of_timer_stopped_in_callback)
{
    struct Target target = {.context = utest_fixture->context, .timer = utest_fixture->timer, .timeout = 0};

    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_callback(utest_fixture->timer, stop_target, &target);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(target.calls.count, 1);
    EXPECT_FALSE(vmnl_net_timer_is_active(utest_fixture->timer));
}

UTEST_F(vmnl_net_context_advance, skips_rearm_of_timer_started_in_callback)
{
    struct Target target = {.context = utest_fixture->context, .timer = utest_fixture->timer, .timeout = 0};

    vmnl_net_timer_set_interval(utest_fixture->timer, TIMEOUT);
    vmnl_net_timer_set_callback(utest_fixture->timer, start_target, &target);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(target.calls.count, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(utest_fixture->timer, START_TIME + TIMEOUT), UINT64_C(0));
}

UTEST_F(vmnl_net_context_advance, calls_timer_started_in_callback_at_next_call)
{
    struct Target target = {.context = utest_fixture->context, .timer = utest_fixture->timer, .timeout = 0};

    vmnl_net_timer_set_callback(utest_fixture->timer, start_target, &target);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(target.calls.count, 1);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    EXPECT_EQ(target.calls.count, 2);
}

UTEST_F(vmnl_net_context_advance, starts_timers_from_now_in_callbacks)
{
    struct Calls calls = {0};
    struct Target target = {.context = utest_fixture->context, .timeout = TIMEOUT};

    target.timer = vmnl_net_timer_create(count_calls, &calls, NULL);
    ASSERT_NE(target.timer, NULL);
    vmnl_net_timer_set_callback(utest_fixture->timer, start_target, &target);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + 5 * TIMEOUT);
    ASSERT_EQ(target.calls.count, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(target.timer, START_TIME + 5 * TIMEOUT), TIMEOUT);
    vmnl_net_timer_destroy(target.timer);
}

UTEST_F(vmnl_net_context_advance, skips_timer_stopped_by_earlier_callback)
{
    struct Calls calls = {0};
    struct Target target = {.context = utest_fixture->context, .timeout = 0};

    target.timer = vmnl_net_timer_create(count_calls, &calls, NULL);
    ASSERT_NE(target.timer, NULL);
    vmnl_net_timer_set_callback(utest_fixture->timer, stop_target, &target);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT / 2);
    vmnl_net_timer_start(utest_fixture->context, target.timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(target.calls.count, 1);
    EXPECT_EQ(calls.count, 0);
    vmnl_net_timer_destroy(target.timer);
}

UTEST_F(vmnl_net_context_advance, skips_timer_destroyed_by_earlier_callback)
{
    struct Calls calls = {0};
    struct Target target = {.context = utest_fixture->context, .timeout = 0};

    target.timer = vmnl_net_timer_create(count_calls, &calls, NULL);
    ASSERT_NE(target.timer, NULL);
    vmnl_net_timer_set_callback(utest_fixture->timer, destroy_target, &target);
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT / 2);
    vmnl_net_timer_start(utest_fixture->context, target.timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(target.calls.count, 1);
    EXPECT_EQ(calls.count, 0);
}

UTEST_F(vmnl_net_context_advance, skips_timer_before_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT - 1);
    EXPECT_EQ(utest_fixture->calls.count, 0);
}

UTEST_F(vmnl_net_context_advance, passes_zero_lateness_at_deadline)
{
    vmnl_net_timer_start(utest_fixture->context, utest_fixture->timer, TIMEOUT);
    utest_fixture->calls.lateness = 1;
    vmnl_net_context_advance(utest_fixture->context, START_TIME + TIMEOUT);
    ASSERT_EQ(utest_fixture->calls.count, 1);
    EXPECT_EQ(utest_fixture->calls.lateness, UINT64_C(0));
}
