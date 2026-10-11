#include <vmnl/net/timer.h>

#include <gtest/gtest.h>

static size_t calls = 0;

class VmnlNetTimerRemaining : public testing::Test {
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

TEST_F(VmnlNetTimerRemaining, Success)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 10), 40);
}

TEST_F(VmnlNetTimerRemaining, GivesTheEarliestDeadline)
{
    VmnlNetTimer *earlier = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) {}, nullptr, nullptr);

    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_start(context, earlier, 0, 20, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), 20);
    vmnl_net_timer_destroy(context, earlier);
}

TEST_F(VmnlNetTimerRemaining, DueTimer)
{
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 60), 0);
}

TEST_F(VmnlNetTimerRemaining, NoRunningTimer)
{
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), UINT64_MAX);
}

TEST_F(VmnlNetTimerRemaining, NullContext)
{
    EXPECT_EQ(vmnl_net_timer_remaining(nullptr, 0), UINT64_MAX);
}
