#include <vmnl/net/context.h>

#include <gtest/gtest.h>

TEST(VmnlNetContextCreate, Success)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetContext *context = vmnl_net_context_create(&error);

    EXPECT_NE(context, nullptr);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    vmnl_net_context_destroy(context);
}

TEST(VmnlNetContextCreate, NullError)
{
    VmnlNetContext *context = vmnl_net_context_create(nullptr);

    EXPECT_NE(context, nullptr);
    vmnl_net_context_destroy(context);
}
