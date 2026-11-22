#include "error/error_internal.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "utest.h"

UTEST(vmnl_net_error_from_system, returns_enomem_with_wsa_not_enough_memory)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSA_NOT_ENOUGH_MEMORY), VMNL_NET_ENOMEM);
}

UTEST(vmnl_net_error_from_system, returns_eintr_with_wsaeintr)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEINTR), VMNL_NET_EINTR);
}

UTEST(vmnl_net_error_from_system, returns_eacces_with_wsaeacces)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEACCES), VMNL_NET_EACCES);
}

UTEST(vmnl_net_error_from_system, returns_einval_with_wsaeinval)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEINVAL), VMNL_NET_EINVAL);
}

UTEST(vmnl_net_error_from_system, returns_emfile_with_wsaemfile)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEMFILE), VMNL_NET_EMFILE);
}

UTEST(vmnl_net_error_from_system, returns_eagain_with_wsaewouldblock)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEWOULDBLOCK), VMNL_NET_EAGAIN);
}

UTEST(vmnl_net_error_from_system, returns_ealready_with_wsaealready)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEALREADY), VMNL_NET_EALREADY);
}

UTEST(vmnl_net_error_from_system, returns_emsgsize_with_wsaemsgsize)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEMSGSIZE), VMNL_NET_EMSGSIZE);
}

UTEST(vmnl_net_error_from_system, returns_enotsup_with_wsaeprotonosupport)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEPROTONOSUPPORT), VMNL_NET_ENOTSUP);
}

UTEST(vmnl_net_error_from_system, returns_enotsup_with_wsaeopnotsupp)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEOPNOTSUPP), VMNL_NET_ENOTSUP);
}

UTEST(vmnl_net_error_from_system, returns_eafnosupport_with_wsaeafnosupport)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEAFNOSUPPORT), VMNL_NET_EAFNOSUPPORT);
}

UTEST(vmnl_net_error_from_system, returns_eaddrinuse_with_wsaeaddrinuse)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEADDRINUSE), VMNL_NET_EADDRINUSE);
}

UTEST(vmnl_net_error_from_system, returns_eaddrnotavail_with_wsaeaddrnotavail)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEADDRNOTAVAIL), VMNL_NET_EADDRNOTAVAIL);
}

UTEST(vmnl_net_error_from_system, returns_enetdown_with_wsaenetdown)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAENETDOWN), VMNL_NET_ENETDOWN);
}

UTEST(vmnl_net_error_from_system, returns_enetunreach_with_wsaenetunreach)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAENETUNREACH), VMNL_NET_ENETUNREACH);
}

UTEST(vmnl_net_error_from_system, returns_enetreset_with_wsaenetreset)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAENETRESET), VMNL_NET_ENETRESET);
}

UTEST(vmnl_net_error_from_system, returns_econnaborted_with_wsaeconnaborted)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAECONNABORTED), VMNL_NET_ECONNABORTED);
}

UTEST(vmnl_net_error_from_system, returns_econnreset_with_wsaeconnreset)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAECONNRESET), VMNL_NET_ECONNRESET);
}

UTEST(vmnl_net_error_from_system, returns_enobufs_with_wsaenobufs)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAENOBUFS), VMNL_NET_ENOBUFS);
}

UTEST(vmnl_net_error_from_system, returns_eisconn_with_wsaeisconn)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEISCONN), VMNL_NET_EISCONN);
}

UTEST(vmnl_net_error_from_system, returns_enotconn_with_wsaenotconn)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAENOTCONN), VMNL_NET_ENOTCONN);
}

UTEST(vmnl_net_error_from_system, returns_epipe_with_wsaeshutdown)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAESHUTDOWN), VMNL_NET_EPIPE);
}

UTEST(vmnl_net_error_from_system, returns_etimedout_with_wsaetimedout)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAETIMEDOUT), VMNL_NET_ETIMEDOUT);
}

UTEST(vmnl_net_error_from_system, returns_econnrefused_with_wsaeconnrefused)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAECONNREFUSED), VMNL_NET_ECONNREFUSED);
}

UTEST(vmnl_net_error_from_system, returns_ehostunreach_with_wsaehostunreach)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEHOSTUNREACH), VMNL_NET_EHOSTUNREACH);
}

UTEST(vmnl_net_error_from_system, returns_fsystem_without_matching_code)
{
    EXPECT_EQ(vmnl_net_error_from_system(WSAEINPROGRESS), VMNL_NET_FSYSTEM);
}
