#include "error/error_internal.h"

#include "utest.h"

UTEST(vmnl_net_error_from_system, returns_enomem_with_enomem)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENOMEM), VMNL_NET_ENOMEM);
}

UTEST(vmnl_net_error_from_system, returns_eintr_with_eintr)
{
    EXPECT_EQ(vmnl_net_error_from_system(EINTR), VMNL_NET_EINTR);
}

UTEST(vmnl_net_error_from_system, returns_eacces_with_eacces)
{
    EXPECT_EQ(vmnl_net_error_from_system(EACCES), VMNL_NET_EACCES);
}

UTEST(vmnl_net_error_from_system, returns_eperm_with_eperm)
{
    EXPECT_EQ(vmnl_net_error_from_system(EPERM), VMNL_NET_EPERM);
}

UTEST(vmnl_net_error_from_system, returns_einval_with_einval)
{
    EXPECT_EQ(vmnl_net_error_from_system(EINVAL), VMNL_NET_EINVAL);
}

UTEST(vmnl_net_error_from_system, returns_emfile_with_emfile)
{
    EXPECT_EQ(vmnl_net_error_from_system(EMFILE), VMNL_NET_EMFILE);
}

UTEST(vmnl_net_error_from_system, returns_enfile_with_enfile)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENFILE), VMNL_NET_ENFILE);
}

UTEST(vmnl_net_error_from_system, returns_eagain_with_eagain)
{
    EXPECT_EQ(vmnl_net_error_from_system(EAGAIN), VMNL_NET_EAGAIN);
}

UTEST(vmnl_net_error_from_system, returns_eagain_with_ewouldblock)
{
    EXPECT_EQ(vmnl_net_error_from_system(EWOULDBLOCK), VMNL_NET_EAGAIN);
}

UTEST(vmnl_net_error_from_system, returns_einprogress_with_einprogress)
{
    EXPECT_EQ(vmnl_net_error_from_system(EINPROGRESS), VMNL_NET_EINPROGRESS);
}

UTEST(vmnl_net_error_from_system, returns_ealready_with_ealready)
{
    EXPECT_EQ(vmnl_net_error_from_system(EALREADY), VMNL_NET_EALREADY);
}

UTEST(vmnl_net_error_from_system, returns_emsgsize_with_emsgsize)
{
    EXPECT_EQ(vmnl_net_error_from_system(EMSGSIZE), VMNL_NET_EMSGSIZE);
}

UTEST(vmnl_net_error_from_system, returns_enotsup_with_eprotonosupport)
{
    EXPECT_EQ(vmnl_net_error_from_system(EPROTONOSUPPORT), VMNL_NET_ENOTSUP);
}

UTEST(vmnl_net_error_from_system, returns_enotsup_with_enotsup)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENOTSUP), VMNL_NET_ENOTSUP);
}

UTEST(vmnl_net_error_from_system, returns_enotsup_with_eopnotsupp)
{
    EXPECT_EQ(vmnl_net_error_from_system(EOPNOTSUPP), VMNL_NET_ENOTSUP);
}

UTEST(vmnl_net_error_from_system, returns_eafnosupport_with_eafnosupport)
{
    EXPECT_EQ(vmnl_net_error_from_system(EAFNOSUPPORT), VMNL_NET_EAFNOSUPPORT);
}

UTEST(vmnl_net_error_from_system, returns_eaddrinuse_with_eaddrinuse)
{
    EXPECT_EQ(vmnl_net_error_from_system(EADDRINUSE), VMNL_NET_EADDRINUSE);
}

UTEST(vmnl_net_error_from_system, returns_eaddrnotavail_with_eaddrnotavail)
{
    EXPECT_EQ(vmnl_net_error_from_system(EADDRNOTAVAIL), VMNL_NET_EADDRNOTAVAIL);
}

UTEST(vmnl_net_error_from_system, returns_enetdown_with_enetdown)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENETDOWN), VMNL_NET_ENETDOWN);
}

UTEST(vmnl_net_error_from_system, returns_enetunreach_with_enetunreach)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENETUNREACH), VMNL_NET_ENETUNREACH);
}

UTEST(vmnl_net_error_from_system, returns_enetreset_with_enetreset)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENETRESET), VMNL_NET_ENETRESET);
}

UTEST(vmnl_net_error_from_system, returns_econnaborted_with_econnaborted)
{
    EXPECT_EQ(vmnl_net_error_from_system(ECONNABORTED), VMNL_NET_ECONNABORTED);
}

UTEST(vmnl_net_error_from_system, returns_econnreset_with_econnreset)
{
    EXPECT_EQ(vmnl_net_error_from_system(ECONNRESET), VMNL_NET_ECONNRESET);
}

UTEST(vmnl_net_error_from_system, returns_enobufs_with_enobufs)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENOBUFS), VMNL_NET_ENOBUFS);
}

UTEST(vmnl_net_error_from_system, returns_eisconn_with_eisconn)
{
    EXPECT_EQ(vmnl_net_error_from_system(EISCONN), VMNL_NET_EISCONN);
}

UTEST(vmnl_net_error_from_system, returns_enotconn_with_enotconn)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENOTCONN), VMNL_NET_ENOTCONN);
}

UTEST(vmnl_net_error_from_system, returns_enotconn_with_edestaddrreq)
{
    EXPECT_EQ(vmnl_net_error_from_system(EDESTADDRREQ), VMNL_NET_ENOTCONN);
}

UTEST(vmnl_net_error_from_system, returns_epipe_with_epipe)
{
    EXPECT_EQ(vmnl_net_error_from_system(EPIPE), VMNL_NET_EPIPE);
}

UTEST(vmnl_net_error_from_system, returns_etimedout_with_etimedout)
{
    EXPECT_EQ(vmnl_net_error_from_system(ETIMEDOUT), VMNL_NET_ETIMEDOUT);
}

UTEST(vmnl_net_error_from_system, returns_econnrefused_with_econnrefused)
{
    EXPECT_EQ(vmnl_net_error_from_system(ECONNREFUSED), VMNL_NET_ECONNREFUSED);
}

UTEST(vmnl_net_error_from_system, returns_ehostunreach_with_ehostunreach)
{
    EXPECT_EQ(vmnl_net_error_from_system(EHOSTUNREACH), VMNL_NET_EHOSTUNREACH);
}

UTEST(vmnl_net_error_from_system, returns_fsystem_without_matching_code)
{
    EXPECT_EQ(vmnl_net_error_from_system(ENOENT), VMNL_NET_FSYSTEM);
}

UTEST(vmnl_net_error_from_system, returns_fsystem_with_int_max)
{
    EXPECT_EQ(vmnl_net_error_from_system(INT_MAX), VMNL_NET_FSYSTEM);
}
