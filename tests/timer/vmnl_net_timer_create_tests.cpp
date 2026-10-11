#include <vmnl/net/timer.h>

#include <gtest/gtest.h>

#include <cstdlib>

static size_t allocations = 0;

class VmnlNetTimerCreate : public testing::Test {
  protected:
    void SetUp() override
    {
        allocations = 0;
    }
};

TEST_F(VmnlNetTimerCreate, Success)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetError error = UINT32_MAX;
    VmnlNetTimer *timer = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) {}, nullptr, &error);

    EXPECT_NE(timer, nullptr);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_timer_remaining(context, 0), UINT64_MAX);
    vmnl_net_timer_destroy(context, timer);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetTimerCreate, AllocationFailure)
{
    VmnlNetAllocator allocator = {[](size_t size) { return allocations++ == 1 ? nullptr : std::malloc(size); }, std::realloc, std::free};
    VmnlNetContext *context = vmnl_net_context_create_with_allocator(&allocator, nullptr);
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) {}, nullptr, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetTimerCreate, NullContext)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_timer_create(nullptr, [](VmnlNetTimer *, void *) {}, nullptr, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

TEST_F(VmnlNetTimerCreate, NullCallback)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetError error = UINT32_MAX;

    EXPECT_EQ(vmnl_net_timer_create(context, nullptr, nullptr, &error), nullptr);
    EXPECT_EQ(error, VMNL_NET_EINVAL);
    vmnl_net_context_destroy(context);
}

TEST_F(VmnlNetTimerCreate, NullError)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);
    VmnlNetTimer *timer = vmnl_net_timer_create(context, [](VmnlNetTimer *, void *) {}, nullptr, nullptr);

    EXPECT_NE(timer, nullptr);
    vmnl_net_timer_destroy(context, timer);
    vmnl_net_context_destroy(context);
}
