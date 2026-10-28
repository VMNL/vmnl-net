#include <vmnl/net/version.h>

#include "utest.h"

UTEST(vmnl_net_version_string, returns_current_version)
{
    EXPECT_STREQ(vmnl_net_version_string(), "0.2.0");
}
