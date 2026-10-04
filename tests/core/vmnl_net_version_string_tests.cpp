#include <vmnl/net/core.h>

#include <gtest/gtest.h>

TEST(VmnlNetVersionString, ProjectVersion)
{
    EXPECT_STREQ(vmnl_net_version_string(), "0.1.0");
}
