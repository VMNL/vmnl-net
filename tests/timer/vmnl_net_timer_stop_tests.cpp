#include <vmnl/net/timer.h>

#include <gtest/gtest.h>

static size_t calls = 0;

class VmnlNetTimerStop : public testing::Test {
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

TEST_F(VmnlNetTimerStop, Success)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_stop(context, timer);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), UINT64_MAX);
    vmnl_net_timer_update(context, 100);
    EXPECT_EQ(calls, 0);
}

TEST_F(VmnlNetTimerStop, StoppedTimer)
{
    vmnl_net_timer_stop(context, timer);
    vmnl_net_timer_stop(context, timer);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), UINT64_MAX);
}

TEST_F(VmnlNetTimerStop, NullContext)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_stop(nullptr, timer);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), 50);
}

TEST_F(VmnlNetTimerStop, NullTimer)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_stop(context, nullptr);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), 50);
}
