#include <vmnl/net/context.h>

#include <gtest/gtest.h>

#include <cstdlib>

static void *failing_allocate(size_t)
{
    return nullptr;
}

TEST(VmnlNetContextCreate, Success)
{
    VmnlNetContext *context = nullptr;

    EXPECT_EQ(vmnl_net_context_create(&context), VMNL_NET_SUCCESS);
    EXPECT_NE(context, nullptr);
    vmnl_net_context_destroy(context);
}

TEST(VmnlNetContextCreate, NullContext)
{
    EXPECT_EQ(vmnl_net_context_create(nullptr), VMNL_NET_EINVAL);
}

TEST(VmnlNetContextCreate, AllocationFailure)
{
    static char sentinel;
    VmnlNetContext *context = reinterpret_cast<VmnlNetContext *>(&sentinel);

    ASSERT_EQ(vmnl_net_set_allocator(failing_allocate, std::realloc, std::free), VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_context_create(&context), VMNL_NET_ENOMEM);
    EXPECT_EQ(context, nullptr);
    vmnl_net_set_allocator(std::malloc, std::realloc, std::free);
}
