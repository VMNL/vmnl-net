#include "error/error_internal.h"

#include <errno.h>
#include <stddef.h>

struct VmnlNetErrnoErrorsMapping {
    int errno_error;
    VmnlNetError vmnl_net_error;
};

static const struct VmnlNetErrnoErrorsMapping vmnl_net_error_system_table[] = {
    {ENOMEM, VMNL_NET_ENOMEM},
    {EINTR, VMNL_NET_EINTR},
    {EACCES, VMNL_NET_EACCES},
    {EPERM, VMNL_NET_EPERM},
    {EINVAL, VMNL_NET_EINVAL},
    {EMFILE, VMNL_NET_EMFILE},
    {ENFILE, VMNL_NET_ENFILE},
    {EAGAIN, VMNL_NET_EAGAIN},
#if EWOULDBLOCK != EAGAIN
    {EWOULDBLOCK, VMNL_NET_EAGAIN},
#endif
    {EINPROGRESS, VMNL_NET_EINPROGRESS},
    {EALREADY, VMNL_NET_EALREADY},
    {EMSGSIZE, VMNL_NET_EMSGSIZE},
    {EPROTONOSUPPORT, VMNL_NET_ENOTSUP},
    {ENOTSUP, VMNL_NET_ENOTSUP},
#if EOPNOTSUPP != ENOTSUP
    {EOPNOTSUPP, VMNL_NET_ENOTSUP},
#endif
    {EAFNOSUPPORT, VMNL_NET_EAFNOSUPPORT},
    {EADDRINUSE, VMNL_NET_EADDRINUSE},
    {EADDRNOTAVAIL, VMNL_NET_EADDRNOTAVAIL},
    {ENETDOWN, VMNL_NET_ENETDOWN},
    {ENETUNREACH, VMNL_NET_ENETUNREACH},
    {ENETRESET, VMNL_NET_ENETRESET},
    {ECONNABORTED, VMNL_NET_ECONNABORTED},
    {ECONNRESET, VMNL_NET_ECONNRESET},
    {ENOBUFS, VMNL_NET_ENOBUFS},
    {EISCONN, VMNL_NET_EISCONN},
    {ENOTCONN, VMNL_NET_ENOTCONN},
    {EPIPE, VMNL_NET_EPIPE},
    {ETIMEDOUT, VMNL_NET_ETIMEDOUT},
    {ECONNREFUSED, VMNL_NET_ECONNREFUSED},
    {EHOSTUNREACH, VMNL_NET_EHOSTUNREACH},
};

VmnlNetError vmnl_net_error_from_system(int error_code)
{
    for (size_t it = 0; it < sizeof vmnl_net_error_system_table / sizeof vmnl_net_error_system_table[0]; it++) {
        if (vmnl_net_error_system_table[it].errno_error == error_code) {
            return vmnl_net_error_system_table[it].vmnl_net_error;
        }
    }
    return VMNL_NET_FSYSTEM;
}
