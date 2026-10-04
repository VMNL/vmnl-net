#include <vmnl/net/core.h>

#include <gtest/gtest.h>

TEST(VmnlNetErrorString, Success)
{
    EXPECT_STREQ(vmnl_net_error_string(VMNL_NET_SUCCESS), "Success");
}

TEST(VmnlNetErrorString, Einval)
{
    EXPECT_STREQ(vmnl_net_error_string(VMNL_NET_EINVAL), "Invalid argument");
}

TEST(VmnlNetErrorString, Enomem)
{
    EXPECT_STREQ(vmnl_net_error_string(VMNL_NET_ENOMEM), "Cannot allocate memory");
}

TEST(VmnlNetErrorString, Fsystem)
{
    EXPECT_STREQ(vmnl_net_error_string(VMNL_NET_FSYSTEM), "System error");
}

TEST(VmnlNetErrorString, FirstUnknownCode)
{
    EXPECT_STREQ(vmnl_net_error_string(VMNL_NET_FSYSTEM + 1), "Unknown error");
}

TEST(VmnlNetErrorString, LargestCode)
{
    EXPECT_STREQ(vmnl_net_error_string(UINT32_MAX), "Unknown error");
}
