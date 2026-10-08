#include <vmnl/net/error.h>

#include <gtest/gtest.h>

#include <cstring>

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

TEST(VmnlNetErrorString, EveryKnownCodeHasAMessage)
{
    for (VmnlNetError error = VMNL_NET_SUCCESS;; ++error) {
        const char *message = vmnl_net_error_string(error);

        ASSERT_NE(message, nullptr) << "code " << error;
        if (std::strcmp(message, "Unknown error") == 0) {
            break;
        }
    }
}
