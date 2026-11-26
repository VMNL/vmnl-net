#include "address/address_internal.h"
#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stddef.h>

size_t vmnl_net_socket_receive_from(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    /* NOLINTNEXTLINE(readability-redundant-parentheses) */
    struct msghdr message = {.msg_name = &storage, .msg_namelen = sizeof storage, .msg_iov = (struct iovec *) views, .msg_iovlen = (__typeof__(message.msg_iovlen)) nviews};
    ssize_t nbytes;

    if (nviews == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return 0;
    }
    nbytes = recvmsg(netsock->handle, &message, 0);
    if (nbytes == -1) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return 0;
    }
    if (((unsigned int) message.msg_flags & (unsigned int) MSG_TRUNC) != 0) {
        vmnl_net_error_set(error, VMNL_NET_EMSGSIZE);
        return 0;
    }
    if (address != NULL) {
        (void) vmnl_net_address_from_sockaddr(&storage, address);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return (size_t) nbytes;
}
