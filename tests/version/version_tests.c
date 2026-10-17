#include <vmnl/net/version.h>

#include "utest.h"

#include <stdint.h>
#include <stdio.h>

UTEST(vmnl_net_version, success)
{
    const uint32_t version = vmnl_net_version();
    const uint32_t major = VMNL_NET_TEST_VERSION_MAJOR;
    const uint32_t minor = VMNL_NET_TEST_VERSION_MINOR;
    const uint32_t patch = VMNL_NET_TEST_VERSION_PATCH;

    EXPECT_EQ(VMNL_NET_VERSION_GET_MAJOR(version), major);
    EXPECT_EQ(VMNL_NET_VERSION_GET_MINOR(version), minor);
    EXPECT_EQ(VMNL_NET_VERSION_GET_PATCH(version), patch);
}

UTEST(vmnl_net_version, matches_the_string)
{
    const uint32_t version = vmnl_net_version();
    char text[16];

    snprintf(text, sizeof text, "%u.%u.%u", (unsigned) VMNL_NET_VERSION_GET_MAJOR(version), (unsigned) VMNL_NET_VERSION_GET_MINOR(version), (unsigned) VMNL_NET_VERSION_GET_PATCH(version));
    EXPECT_STREQ(text, vmnl_net_version_string());
}

UTEST(vmnl_net_version, unpacks_with_the_macros)
{
    const uint32_t version = UINT32_C(0x010203);

    EXPECT_EQ(VMNL_NET_VERSION_GET_MAJOR(version), UINT32_C(1));
    EXPECT_EQ(VMNL_NET_VERSION_GET_MINOR(version), UINT32_C(2));
    EXPECT_EQ(VMNL_NET_VERSION_GET_PATCH(version), UINT32_C(3));
}
