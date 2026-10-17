#include <vmnl/net/version.h>

#include "utest.h"

UTEST(vmnl_net_version_string, success)
{
    EXPECT_STREQ(vmnl_net_version_string(), VMNL_NET_TEST_VERSION);
}
