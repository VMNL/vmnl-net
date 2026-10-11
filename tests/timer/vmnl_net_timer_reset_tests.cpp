#include <vmnl/net/timer.h>

#include <gtest/gtest.h>

static size_t calls = 0;

class VmnlNetTimerReset : public testing::Test {
  protected:
    void SetUp() override
    {
        calls = 0;
        context = vmnl_net_context_create(nullptr);
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

TEST_F(VmnlNetTimerReset, Success)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_reset(context, timer, 30);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 30), 50);
    vmnl_net_timer_update(context, 51);
    EXPECT_EQ(calls, 0);
    vmnl_net_timer_update(context, 81);
    EXPECT_EQ(calls, 1);
}

TEST_F(VmnlNetTimerReset, RestartsAStoppedTimer)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_stop(context, timer);
    vmnl_net_timer_reset(context, timer, 100);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 100), 50);
}

TEST_F(VmnlNetTimerReset, NeverStarted)
{
    vmnl_net_timer_reset(context, timer, 100);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 100), 0);
    vmnl_net_timer_update(context, 101);
    EXPECT_EQ(calls, 1);
}

TEST_F(VmnlNetTimerReset, NullContext)
{
    vmnl_net_timer_reset(nullptr, timer, 100);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 100), UINT64_MAX);
}

TEST_F(VmnlNetTimerReset, NullTimer)
{
    vmnl_net_timer_reset(context, nullptr, 100);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 100), UINT64_MAX);
}
