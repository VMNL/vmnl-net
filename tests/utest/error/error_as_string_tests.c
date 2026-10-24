#include <vmnl/net/error.h>

#include "utest.h"

UTEST(vmnl_net_error_as_string, returns_success_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_SUCCESS), "Success");
}

UTEST(vmnl_net_error_as_string, returns_einval_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EINVAL), "Invalid argument");
}

UTEST(vmnl_net_error_as_string, returns_enomem_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOMEM), "Cannot allocate memory");
}

UTEST(vmnl_net_error_as_string, returns_enobufs_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOBUFS), "No buffer space available");
}

UTEST(vmnl_net_error_as_string, returns_eagain_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAGAIN), "Resource temporarily unavailable");
}

UTEST(vmnl_net_error_as_string, returns_eintr_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EINTR), "Interrupted system call");
}

UTEST(vmnl_net_error_as_string, returns_enotsup_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOTSUP), "Operation not supported");
}

UTEST(vmnl_net_error_as_string, returns_eacces_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EACCES), "Permission denied");
}

UTEST(vmnl_net_error_as_string, returns_eperm_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EPERM), "Operation not permitted");
}

UTEST(vmnl_net_error_as_string, returns_emfile_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EMFILE), "Too many open files");
}

UTEST(vmnl_net_error_as_string, returns_enfile_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENFILE), "Too many open files in system");
}

UTEST(vmnl_net_error_as_string, returns_eafnosupport_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAFNOSUPPORT), "Address family not supported");
}

UTEST(vmnl_net_error_as_string, returns_eaddrinuse_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EADDRINUSE), "Address already in use");
}

UTEST(vmnl_net_error_as_string, returns_eaddrnotavail_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EADDRNOTAVAIL), "Cannot assign requested address");
}

UTEST(vmnl_net_error_as_string, returns_enetdown_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENETDOWN), "Network is down");
}

UTEST(vmnl_net_error_as_string, returns_enetunreach_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENETUNREACH), "Network is unreachable");
}

UTEST(vmnl_net_error_as_string, returns_enetreset_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENETRESET), "Network dropped connection on reset");
}

UTEST(vmnl_net_error_as_string, returns_ehostunreach_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EHOSTUNREACH), "No route to host");
}

UTEST(vmnl_net_error_as_string, returns_einprogress_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EINPROGRESS), "Operation now in progress");
}

UTEST(vmnl_net_error_as_string, returns_ealready_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EALREADY), "Operation already in progress");
}

UTEST(vmnl_net_error_as_string, returns_eisconn_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EISCONN), "Socket is already connected");
}

UTEST(vmnl_net_error_as_string, returns_enotconn_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ENOTCONN), "Socket is not connected");
}

UTEST(vmnl_net_error_as_string, returns_econnrefused_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ECONNREFUSED), "Connection refused");
}

UTEST(vmnl_net_error_as_string, returns_econnreset_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ECONNRESET), "Connection reset by peer");
}

UTEST(vmnl_net_error_as_string, returns_econnaborted_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ECONNABORTED), "Connection aborted");
}

UTEST(vmnl_net_error_as_string, returns_etimedout_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_ETIMEDOUT), "Connection timed out");
}

UTEST(vmnl_net_error_as_string, returns_epipe_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EPIPE), "Broken pipe");
}

UTEST(vmnl_net_error_as_string, returns_emsgsize_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EMSGSIZE), "Message too long");
}

UTEST(vmnl_net_error_as_string, returns_eai_noname_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_NONAME), "Unknown host or service");
}

UTEST(vmnl_net_error_as_string, returns_eai_again_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_AGAIN), "Temporary failure in name resolution");
}

UTEST(vmnl_net_error_as_string, returns_eai_fail_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_FAIL), "Non-recoverable failure in name resolution");
}

UTEST(vmnl_net_error_as_string, returns_fsystem_message)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_FSYSTEM), "System error");
}

UTEST(vmnl_net_error_as_string, handles_unused_code)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_EAI_FAIL + 1), "Unknown error");
}

UTEST(vmnl_net_error_as_string, handles_past_last_code)
{
    EXPECT_STREQ(vmnl_net_error_as_string(VMNL_NET_FSYSTEM + 1), "Unknown error");
}

UTEST(vmnl_net_error_as_string, handles_largest_code)
{
    EXPECT_STREQ(vmnl_net_error_as_string(UINT32_MAX), "Unknown error");
}
