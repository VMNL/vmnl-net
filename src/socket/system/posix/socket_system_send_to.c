#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

#include <errno.h>

size_t vmnl_net_socket_system_send_to(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    /* NOLINTNEXTLINE(readability-redundant-parentheses) */
    struct msghdr message = {.msg_name = (address == NULL) ? NULL : &storage, .msg_iov = (struct iovec *) views, .msg_iovlen = (__typeof__(message.msg_iovlen)) nviews};
    ssize_t nbytes;

    if (vmnl_net_socket_system_check_views(views, nviews, error) == false) {
        return 0;
    }
    if (address != NULL && (netsock->flags & VMNL_NET_SOCKET_SYSTEM_FLAG_CONNECTED) != 0) {
        vmnl_net_error_set(error, VMNL_NET_EISCONN);
        return 0;
    }
    if (address != NULL && vmnl_net_socket_system_check_remote(netsock, address, error) == false) {
        return 0;
    }
    if (address != NULL && vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &message.msg_namelen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return 0;
    }
    nbytes = sendmsg(netsock->handle, &message, MSG_NOSIGNAL);
    if (nbytes == -1 && errno == ECONNREFUSED && netsock->type == SOCK_DGRAM) {
        netsock->flags |= VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED;
        nbytes = sendmsg(netsock->handle, &message, MSG_NOSIGNAL);
    }
    if (nbytes == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return 0;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return (size_t) nbytes;
}
