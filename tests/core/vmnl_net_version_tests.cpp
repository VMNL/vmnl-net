#include <vmnl/net/core.h>

#include <gtest/gtest.h>

TEST(VmnlNetVersion, ProjectVersion)
{
    EXPECT_EQ(vmnl_net_version(), 0x000200);
}
