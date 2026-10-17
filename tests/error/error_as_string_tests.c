#include <vmnl/net/error.h>

#include "utest.h"

#include <stdint.h>

UTEST(vmnl_net_error_as_string, success)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_SUCCESS), "Success");
}

UTEST(vmnl_net_error_as_string, einval)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EINVAL), "Invalid argument");
}

UTEST(vmnl_net_error_as_string, enomem)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOMEM), "Cannot allocate memory");
}

UTEST(vmnl_net_error_as_string, enobufs)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOBUFS), "No buffer space available");
}

UTEST(vmnl_net_error_as_string, eagain)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAGAIN), "Resource temporarily unavailable");
}

UTEST(vmnl_net_error_as_string, eintr)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EINTR), "Interrupted system call");
}

UTEST(vmnl_net_error_as_string, enotsup)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOTSUP), "Operation not supported");
}

UTEST(vmnl_net_error_as_string, eacces)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EACCES), "Permission denied");
}

UTEST(vmnl_net_error_as_string, eperm)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EPERM), "Operation not permitted");
}

UTEST(vmnl_net_error_as_string, emfile)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EMFILE), "Too many open files");
}

UTEST(vmnl_net_error_as_string, enfile)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENFILE), "Too many open files in system");
}

UTEST(vmnl_net_error_as_string, eafnosupport)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAFNOSUPPORT), "Address family not supported");
}

UTEST(vmnl_net_error_as_string, eaddrinuse)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EADDRINUSE), "Address already in use");
}

UTEST(vmnl_net_error_as_string, eaddrnotavail)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EADDRNOTAVAIL), "Cannot assign requested address");
}

UTEST(vmnl_net_error_as_string, enetdown)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENETDOWN), "Network is down");
}

UTEST(vmnl_net_error_as_string, enetunreach)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENETUNREACH), "Network is unreachable");
}

UTEST(vmnl_net_error_as_string, enetreset)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENETRESET), "Network dropped connection on reset");
}

UTEST(vmnl_net_error_as_string, ehostunreach)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EHOSTUNREACH), "No route to host");
}

UTEST(vmnl_net_error_as_string, einprogress)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EINPROGRESS), "Operation now in progress");
}

UTEST(vmnl_net_error_as_string, ealready)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EALREADY), "Operation already in progress");
}

UTEST(vmnl_net_error_as_string, eisconn)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EISCONN), "Socket is already connected");
}

UTEST(vmnl_net_error_as_string, enotconn)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOTCONN), "Socket is not connected");
}

UTEST(vmnl_net_error_as_string, econnrefused)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ECONNREFUSED), "Connection refused");
}

UTEST(vmnl_net_error_as_string, econnreset)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ECONNRESET), "Connection reset by peer");
}

UTEST(vmnl_net_error_as_string, econnaborted)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ECONNABORTED), "Connection aborted");
}

UTEST(vmnl_net_error_as_string, etimedout)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ETIMEDOUT), "Connection timed out");
}

UTEST(vmnl_net_error_as_string, epipe)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EPIPE), "Broken pipe");
}

UTEST(vmnl_net_error_as_string, emsgsize)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EMSGSIZE), "Message too long");
}

UTEST(vmnl_net_error_as_string, eai_noname)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_NONAME), "Unknown host or service");
}

UTEST(vmnl_net_error_as_string, eai_again)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_AGAIN), "Temporary failure in name resolution");
}

UTEST(vmnl_net_error_as_string, eai_fail)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_FAIL), "Non-recoverable failure in name resolution");
}

UTEST(vmnl_net_error_as_string, fsystem)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_FSYSTEM), "System error");
}

UTEST(vmnl_net_error_as_string, unused_code)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_FAIL + 1), "Unknown error");
}

UTEST(vmnl_net_error_as_string, first_code_past_the_last)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_FSYSTEM + 1), "Unknown error");
}

UTEST(vmnl_net_error_as_string, largest_code)
{
    EXPECT_STREQ(vmnl_net_error_as_string(UINT32_MAX), "Unknown error");
}
