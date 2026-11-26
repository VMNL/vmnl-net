#include "error/error_internal.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "utest.h"

UTEST(vmnl_net_error_from_last_system, returns_eintr_with_wsaeintr)
{
    VmnlNetError error;

    WSASetLastError(WSAEINTR);
    error = vmnl_net_error_from_last_system();
    EXPECT_EQ(error, VMNL_NET_EINTR);
}

UTEST(vmnl_net_error_from_last_system, returns_fsystem_without_matching_code)
{
    VmnlNetError error;

    WSASetLastError(WSAEINPROGRESS);
    error = vmnl_net_error_from_last_system();
    EXPECT_EQ(error, VMNL_NET_FSYSTEM);
}
