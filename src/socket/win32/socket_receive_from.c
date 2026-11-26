#include "address/address_internal.h"
#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stddef.h>

size_t vmnl_net_socket_receive_from(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = sizeof storage;
    DWORD nbytes = 0;
    DWORD flags = 0;
    int code;

    if (nviews == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return 0;
    }
    if (WSARecvFrom(netsock->handle, (LPWSABUF) views, (DWORD) nviews, &nbytes, &flags, (address == NULL) ? NULL : (struct sockaddr *) &storage, (address == NULL) ? NULL : &socklen, NULL, NULL) == SOCKET_ERROR) {
        code = WSAGetLastError();
        vmnl_net_error_set(error, (netsock->type == SOCK_DGRAM && code == WSAECONNRESET) ? VMNL_NET_ECONNREFUSED : vmnl_net_error_from_system(code));
        return 0;
    }
    if (address != NULL) {
        (void) vmnl_net_address_from_sockaddr(&storage, address);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return nbytes;
}
