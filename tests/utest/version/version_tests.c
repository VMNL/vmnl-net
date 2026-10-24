#include <vmnl/net/version.h>

#include "utest.h"

UTEST(vmnl_net_version, returns_current_version)
{
    const uint32_t version = vmnl_net_version();

    EXPECT_EQ(VMNL_NET_VERSION_GET_MAJOR(version), (uint32_t) 0);
    EXPECT_EQ(VMNL_NET_VERSION_GET_MINOR(version), (uint32_t) 2);
    EXPECT_EQ(VMNL_NET_VERSION_GET_PATCH(version), (uint32_t) 0);
}

UTEST(vmnl_net_version, matches_vmnl_net_version_string)
{
    const uint32_t version = vmnl_net_version();
    char buffer[64] = {0};

    snprintf(buffer, sizeof buffer, "%d.%d.%d",
        (int) VMNL_NET_VERSION_GET_MAJOR(version),
        (int) VMNL_NET_VERSION_GET_MINOR(version),
        (int) VMNL_NET_VERSION_GET_PATCH(version)
    );
    EXPECT_STREQ(buffer, vmnl_net_version_string());
}

UTEST(vmnl_net_version, unpacks_with_macros)
{
    const uint32_t version = UINT32_C(0x010203);

    EXPECT_EQ(VMNL_NET_VERSION_GET_MAJOR(version), UINT32_C(1));
    EXPECT_EQ(VMNL_NET_VERSION_GET_MINOR(version), UINT32_C(2));
    EXPECT_EQ(VMNL_NET_VERSION_GET_PATCH(version), UINT32_C(3));
}
