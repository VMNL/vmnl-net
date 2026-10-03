#include <gtest/gtest.h>

#include "vmnl/net/core.h"

TEST(VmnlNetVersionString, ProjectVersion)
{
    EXPECT_STREQ(vmnl_net_version_string(), "0.1.0");
}
