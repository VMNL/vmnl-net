#include "address/address_internal.h"
#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stddef.h>

size_t vmnl_net_socket_send_to(struct VmnlNetSocket *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = 0;
    DWORD nbytes = 0;
    int code;

    if (nviews == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return 0;
    }
    if (address != NULL && vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &socklen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return 0;
    }
    if (WSASendTo(netsock->handle, (LPWSABUF) views, (DWORD) nviews, &nbytes, 0, (address == NULL) ? NULL : (const struct sockaddr *) &storage, socklen, NULL, NULL) == SOCKET_ERROR) {
        code = WSAGetLastError();
        vmnl_net_error_set(error, (netsock->type == SOCK_DGRAM && code == WSAECONNRESET) ? VMNL_NET_ECONNREFUSED : vmnl_net_error_from_system(code));
        return 0;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return nbytes;
}
