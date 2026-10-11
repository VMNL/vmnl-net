#include <vmnl/net/timer.h>

#include <gtest/gtest.h>

static size_t calls = 0;

class VmnlNetTimerStart : public testing::Test {
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

TEST_F(VmnlNetTimerStart, Success)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), 50);
    vmnl_net_timer_update(context, 50);
    EXPECT_EQ(calls, 0);
    vmnl_net_timer_update(context, 51);
    EXPECT_EQ(calls, 1);
}

TEST_F(VmnlNetTimerStart, RestartsARunningTimer)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_start(context, timer, 0, 10, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), 10);
    vmnl_net_timer_update(context, 100);
    EXPECT_EQ(calls, 1);
}

TEST_F(VmnlNetTimerStart, ZeroTimeout)
{
    vmnl_net_timer_start(context, timer, 100, 0, 0);
    vmnl_net_timer_update(context, 100);
    EXPECT_EQ(calls, 0);
    vmnl_net_timer_update(context, 101);
    EXPECT_EQ(calls, 1);
}

TEST_F(VmnlNetTimerStart, MaximumTimeout)
{
    vmnl_net_timer_start(context, timer, 1000, UINT64_MAX, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 1000), UINT64_MAX - 1000);
    vmnl_net_timer_update(context, UINT64_MAX);
    EXPECT_EQ(calls, 0);
}

TEST_F(VmnlNetTimerStart, NullContext)
{
    vmnl_net_timer_start(nullptr, timer, 0, 50, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), UINT64_MAX);
}

TEST_F(VmnlNetTimerStart, NullTimer)
{
    vmnl_net_timer_start(context, nullptr, 0, 50, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), UINT64_MAX);
}
