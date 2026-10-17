#include <vmnl/net/error.h>

#include <stddef.h>

static const char *const vmnl_net_error_table[] = {
    [VMNL_NET_SUCCESS] = "Success",
    [VMNL_NET_EINVAL] = "Invalid argument",
    [VMNL_NET_ENOMEM] = "Cannot allocate memory",
    [VMNL_NET_ENOBUFS] = "No buffer space available",
    [VMNL_NET_EAGAIN] = "Resource temporarily unavailable",
    [VMNL_NET_EINTR] = "Interrupted system call",
    [VMNL_NET_ENOTSUP] = "Operation not supported",
    [VMNL_NET_EACCES] = "Permission denied",
    [VMNL_NET_EPERM] = "Operation not permitted",
    [VMNL_NET_EMFILE] = "Too many open files",
    [VMNL_NET_ENFILE] = "Too many open files in system",
    [VMNL_NET_EAFNOSUPPORT] = "Address family not supported",
    [VMNL_NET_EADDRINUSE] = "Address already in use",
    [VMNL_NET_EADDRNOTAVAIL] = "Cannot assign requested address",
    [VMNL_NET_ENETDOWN] = "Network is down",
    [VMNL_NET_ENETUNREACH] = "Network is unreachable",
    [VMNL_NET_ENETRESET] = "Network dropped connection on reset",
    [VMNL_NET_EHOSTUNREACH] = "No route to host",
    [VMNL_NET_EINPROGRESS] = "Operation now in progress",
    [VMNL_NET_EALREADY] = "Operation already in progress",
    [VMNL_NET_EISCONN] = "Socket is already connected",
    [VMNL_NET_ENOTCONN] = "Socket is not connected",
    [VMNL_NET_ECONNREFUSED] = "Connection refused",
    [VMNL_NET_ECONNRESET] = "Connection reset by peer",
    [VMNL_NET_ECONNABORTED] = "Connection aborted",
    [VMNL_NET_ETIMEDOUT] = "Connection timed out",
    [VMNL_NET_EPIPE] = "Broken pipe",
    [VMNL_NET_EMSGSIZE] = "Message too long",
    [VMNL_NET_EAI_NONAME] = "Unknown host or service",
    [VMNL_NET_EAI_AGAIN] = "Temporary failure in name resolution",
    [VMNL_NET_EAI_FAIL] = "Non-recoverable failure in name resolution",
    [VMNL_NET_FSYSTEM] = "System error",
};

const char *vmnl_net_error_as_string(const VmnlNetError error)
{
    if (error >= sizeof vmnl_net_error_table / sizeof vmnl_net_error_table[0] || vmnl_net_error_table[error] == NULL) {
        return "Unknown error";
    }
    return vmnl_net_error_table[error];
}
