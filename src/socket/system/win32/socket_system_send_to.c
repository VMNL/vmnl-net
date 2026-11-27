#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

size_t vmnl_net_socket_system_send_to(struct VmnlNetSocketSystem *netsock, const VmnlNetAddress *address, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = 0;
    const struct sockaddr *recipient = (address == NULL) ? NULL : (const struct sockaddr *) &storage;
    DWORD nbytes = 0;
    int wsa_error;

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
    if (address != NULL && vmnl_net_address_as_sockaddr(address, netsock->family, &storage, &socklen) == false) {
        vmnl_net_error_set(error, VMNL_NET_EAFNOSUPPORT);
        return 0;
    }
    if (WSASendTo(netsock->handle, (LPWSABUF) views, (DWORD) nviews, &nbytes, 0, recipient, socklen, NULL, NULL) == SOCKET_ERROR) {
        wsa_error = WSAGetLastError();
        vmnl_net_error_set(error, (netsock->type == SOCK_DGRAM && wsa_error == WSAECONNRESET) ? VMNL_NET_ECONNREFUSED : vmnl_net_error_from_system(wsa_error));
        return 0;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return nbytes;
}
