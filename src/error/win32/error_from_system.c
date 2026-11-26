#include "error/error_internal.h"

#include <stddef.h>
#include <winsock2.h>

struct VmnlNetWSAErrorsMapping {
    int wsa_error;
    VmnlNetError vmnl_net_error;
};

static const struct VmnlNetWSAErrorsMapping vmnl_net_error_system_table[] = {
    {WSA_NOT_ENOUGH_MEMORY, VMNL_NET_ENOMEM},
    {WSAEINTR, VMNL_NET_EINTR},
    {WSAEACCES, VMNL_NET_EACCES},
    {WSAEINVAL, VMNL_NET_EINVAL},
    {WSAEMFILE, VMNL_NET_EMFILE},
    {WSAEWOULDBLOCK, VMNL_NET_EAGAIN},
    {WSAEALREADY, VMNL_NET_EALREADY},
    {WSAEMSGSIZE, VMNL_NET_EMSGSIZE},
    {WSAEPROTONOSUPPORT, VMNL_NET_ENOTSUP},
    {WSAEOPNOTSUPP, VMNL_NET_ENOTSUP},
    {WSAEAFNOSUPPORT, VMNL_NET_EAFNOSUPPORT},
    {WSAEADDRINUSE, VMNL_NET_EADDRINUSE},
    {WSAEADDRNOTAVAIL, VMNL_NET_EADDRNOTAVAIL},
    {WSAENETDOWN, VMNL_NET_ENETDOWN},
    {WSAENETUNREACH, VMNL_NET_ENETUNREACH},
    {WSAENETRESET, VMNL_NET_ENETRESET},
    {WSAECONNABORTED, VMNL_NET_ECONNABORTED},
    {WSAECONNRESET, VMNL_NET_ECONNRESET},
    {WSAENOBUFS, VMNL_NET_ENOBUFS},
    {WSAEISCONN, VMNL_NET_EISCONN},
    {WSAENOTCONN, VMNL_NET_ENOTCONN},
    {WSAEDESTADDRREQ, VMNL_NET_ENOTCONN},
    {WSAESHUTDOWN, VMNL_NET_EPIPE},
    {WSAETIMEDOUT, VMNL_NET_ETIMEDOUT},
    {WSAECONNREFUSED, VMNL_NET_ECONNREFUSED},
    {WSAEHOSTUNREACH, VMNL_NET_EHOSTUNREACH},
};

VmnlNetError vmnl_net_error_from_system(int error_code)
{
    for (size_t it = 0; it < sizeof vmnl_net_error_system_table / sizeof vmnl_net_error_system_table[0]; it++) {
        if (vmnl_net_error_system_table[it].wsa_error == error_code) {
            return vmnl_net_error_system_table[it].vmnl_net_error;
        }
    }
    return VMNL_NET_FSYSTEM;
}
