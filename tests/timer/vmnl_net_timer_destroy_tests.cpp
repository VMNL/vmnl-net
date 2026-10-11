#include <vmnl/net/timer.h>

#include <gtest/gtest.h>

#include <cstdlib>

static size_t allocations = 0;
static size_t deallocations = 0;

class VmnlNetTimerDestroy : public testing::Test {
  protected:
    void SetUp() override
    {
        allocations = 0;
        deallocations = 0;
    }
};

static VmnlNetAllocator counting_allocator = {
    [](size_t size) { allocations++; return std::malloc(size); },
    std::realloc,
    [](void *pointer) { deallocations++; std::free(pointer); },
};

TEST_F(VmnlNetTimerDestroy, ReleasesMemory)
{
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&counting_allocator, nullptr);
    VmnlNetTimer *timer = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) {}, nullptr, nullptr);

    ASSERT_NE(timer, nullptr);
    vmnl_net_timer_start(context, timer, 0, 50, 0);
    vmnl_net_timer_destroy(context, timer);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), UINT64_MAX);
    vmnl_net_context_destroy(context);
    EXPECT_EQ(allocations, deallocations);
}

TEST_F(VmnlNetTimerDestroy, NullContext)
{
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&counting_allocator, nullptr);
    VmnlNetTimer *timer = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) {}, nullptr, nullptr);
    size_t released = deallocations;

    vmnl_net_timer_destroy(nullptr, timer);
    EXPECT_EQ(deallocations, released);
    vmnl_net_timer_destroy(context, timer);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetTimerDestroy, NullTimer)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);

    vmnl_net_timer_destroy(context, nullptr);
    vmnl_net_context_destroy(context);
}
