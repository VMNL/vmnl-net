#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

size_t vmnl_net_socket_system_receive_from(struct VmnlNetSocketSystem *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = sizeof storage;
    struct sockaddr *sender = (address == NULL) ? NULL : (struct sockaddr *) &storage;
    socklen_t *sender_length = (address == NULL) ? NULL : &socklen;
    DWORD nbytes = 0;
    int wsa_error;

    if (vmnl_net_socket_system_check_views(views, nviews, error) == false) {
        return 0;
    }
    if (WSARecvFrom(netsock->handle, (LPWSABUF) views, (DWORD) nviews, &nbytes, &(DWORD) {0}, sender, sender_length, NULL, NULL) == SOCKET_ERROR) {
        wsa_error = WSAGetLastError();
        if (netsock->type == SOCK_DGRAM && wsa_error == WSAECONNRESET) {
            vmnl_net_error_set(error, VMNL_NET_ECONNREFUSED);
        } else if (netsock->type == SOCK_DGRAM && wsa_error == WSAEINVAL) {
            vmnl_net_error_set(error, VMNL_NET_EAGAIN);
        } else {
            vmnl_net_error_set(error, vmnl_net_error_from_system(wsa_error));
        }
        return 0;
    }
    if (address != NULL) {
        (void) vmnl_net_address_from_sockaddr(&storage, address);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return nbytes;
}
