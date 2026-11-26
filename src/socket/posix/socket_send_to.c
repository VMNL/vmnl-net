#include "address/address_internal.h"
#include "socket/socket_internal.h"

size_t vmnl_net_socket_send_to(struct VmnlNetSocket *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    /* NOLINTNEXTLINE(readability-redundant-parentheses) */
    struct msghdr message = {.msg_name = (address == NULL) ? NULL : &storage, .msg_iov = (struct iovec *) views, .msg_iovlen = (__typeof__(message.msg_iovlen)) nviews};
    ssize_t nbytes;

    if (nviews == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return 0;
    }
    if (address != NULL && vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &message.msg_namelen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return 0;
    }
    nbytes = sendmsg(netsock->handle, &message, MSG_NOSIGNAL);
    if (nbytes == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return 0;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return (size_t) nbytes;
}
