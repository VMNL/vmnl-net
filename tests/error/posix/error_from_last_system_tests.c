#include "error/error_internal.h"

#include "utest.h"

#include <errno.h>

UTEST(vmnl_net_error_from_last_system, returns_enomem_with_errno_enomem)
{
    VmnlNetError error;

    errno = ENOMEM;
    error = vmnl_net_error_from_last_system();
    EXPECT_EQ(error, VMNL_NET_ENOMEM);
}

UTEST(vmnl_net_error_from_last_system, returns_fsystem_without_matching_errno)
{
    VmnlNetError error;

    errno = ENOENT;
    error = vmnl_net_error_from_last_system();
    EXPECT_EQ(error, VMNL_NET_FSYSTEM);
}
