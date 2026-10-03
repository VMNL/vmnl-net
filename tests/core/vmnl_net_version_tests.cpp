#include <gtest/gtest.h>

#include "vmnl/net/core.h"

TEST(VmnlNetVersion, ProjectVersion)
{
    EXPECT_EQ(vmnl_net_version(), 0x000100);
}
