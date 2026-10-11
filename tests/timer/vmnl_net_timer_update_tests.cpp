#include <vmnl/net/timer.h>

#include <gtest/gtest.h>

static size_t calls = 0;
static VmnlNetContext *shared_context = nullptr;
static VmnlNetTimer *other_timer = nullptr;
static size_t order[4];

class VmnlNetTimerUpdate : public testing::Test {
  protected:
    void SetUp() override
    {
        calls = 0;
        context = vmnl_net_context_create(nullptr);
        shared_context = context;
        timer = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) { calls++; }, nullptr, nullptr);
    }

    void TearDown() override
    {
        vmnl_net_timer_destroy(context, timer);
        vmnl_net_context_destroy(context);
    }

    VmnlNetContext *context;
    VmnlNetTimer *timer;
};

TEST_F(VmnlNetTimerUpdate, Success)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_update(context, 51);
    EXPECT_EQ(calls, 1);
    vmnl_net_timer_update(context, 1000);
    EXPECT_EQ(calls, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 1000), UINT64_MAX);
}

TEST_F(VmnlNetTimerUpdate, ExpiresAfterItsDeadline)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_update(context, 50);
    EXPECT_EQ(calls, 0);
}

TEST_F(VmnlNetTimerUpdate, RepeatsPeriodicTimers)
{
    vmnl_net_timer_start(context, timer, 0, 10, 10);
    vmnl_net_timer_update(context, 11);
    vmnl_net_timer_update(context, 21);
    EXPECT_EQ(calls, 2);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 21), 9);
}

TEST_F(VmnlNetTimerUpdate, MissedPeriodsExpireOnce)
{
    vmnl_net_timer_start(context, timer, 0, 10, 10);
    vmnl_net_timer_update(context, 45);
    EXPECT_EQ(calls, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 45), 10);
}

TEST_F(VmnlNetTimerUpdate, FollowsTheDeadlines)
{
    VmnlNetTimer *timers[3];
    size_t i;

    calls = 0;
    for (i = 0; i < 3; i++) {
        timers[i] = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *user_data) { order[calls++] = (size_t) user_data; }, (void *) i, nullptr);
    }
    vmnl_net_timer_start(context, timers[0], 0, 20, 0);
    vmnl_net_timer_start(context, timers[1], 0, 10, 0);
    vmnl_net_timer_start(context, timers[2], 0, 20, 0);
    vmnl_net_timer_update(context, 100);
    ASSERT_EQ(calls, 3);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 0);
    EXPECT_EQ(order[2], 2);
    for (i = 0; i < 3; i++) {
        vmnl_net_timer_destroy(context, timers[i]);
    }
}

TEST_F(VmnlNetTimerUpdate, RestartedTimerWaitsForTheNextCall)
{
    VmnlNetTimer *restarting = vmnl_net_timer_create(context, [](VmnlNetTimer *self, void *) { calls++; vmnl_net_timer_start(shared_context, self, 100, 0, 0); }, nullptr, nullptr);

    vmnl_net_timer_start(context, restarting, 0, 50, 0);
    vmnl_net_timer_update(context, 100);
    EXPECT_EQ(calls, 1);
    vmnl_net_timer_update(context, 101);
    EXPECT_EQ(calls, 2);
    vmnl_net_timer_destroy(context, restarting);
}

TEST_F(VmnlNetTimerUpdate, DestroyedFromItsCallback)
{
    VmnlNetTimer *destroyed = vmnl_net_timer_create(context, [](VmnlNetTimer *self, void *) { calls++; vmnl_net_timer_destroy(shared_context, self); }, nullptr, nullptr);

    vmnl_net_timer_start(context, destroyed, 0, 10, 10);
    vmnl_net_timer_update(context, 11);
    EXPECT_EQ(calls, 1);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 11), UINT64_MAX);
}

TEST_F(VmnlNetTimerUpdate, StoppedFromAnotherCallback)
{
    VmnlNetTimer *stopper = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) { vmnl_net_timer_stop(shared_context, other_timer); }, nullptr, nullptr);

    other_timer = timer;
    vmnl_net_timer_start(context, stopper, 0, 10, 0);
    vmnl_net_timer_start(context, timer, 0, 10, 0);
    vmnl_net_timer_update(context, 11);
    EXPECT_EQ(calls, 0);
    vmnl_net_timer_destroy(context, stopper);
}

TEST_F(VmnlNetTimerUpdate, NullContext)
{
    vmnl_net_timer_start(context, timer, 0, 10, 0);
    vmnl_net_timer_update(nullptr, 100);
    EXPECT_EQ(calls, 0);
}
