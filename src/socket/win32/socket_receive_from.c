#include "address/address_internal.h"
#include "socket/socket_internal.h"

size_t vmnl_net_socket_receive_from(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = sizeof storage;
    struct sockaddr *sender = (address == NULL) ? NULL : (struct sockaddr *) &storage;
    socklen_t *sender_length = (address == NULL) ? NULL : &socklen;
    DWORD nbytes = 0;
    int wsa_error;

    if (nviews == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return 0;
    }
    if (WSARecvFrom(netsock->handle, (LPWSABUF) views, (DWORD) nviews, &nbytes, &(DWORD) {0}, sender, sender_length, NULL, NULL) == SOCKET_ERROR) {
        wsa_error = WSAGetLastError();
        vmnl_net_error_set(error, (netsock->type == SOCK_DGRAM && wsa_error == WSAECONNRESET) ? VMNL_NET_ECONNREFUSED : vmnl_net_error_from_system(wsa_error));
        return 0;
    }
    if (address != NULL) {
        (void) vmnl_net_address_from_sockaddr(&storage, address);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return nbytes;
}
