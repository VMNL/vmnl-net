#include "address/address_internal.h"
#include "socket/system/socket_system_internal.h"

bool vmnl_net_socket_system_accept(struct VmnlNetSocketSystem *netsock, struct VmnlNetSocketSystem *peer, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = sizeof storage;
    struct VmnlNetSocketSystem sock = {.handle = accept(netsock->handle, (struct sockaddr *) &storage, &socklen)};

    if (sock.handle == VMNL_NET_SOCKET_SYSTEM_INVALID) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    if (vmnl_net_socket_system_non_blocking(&sock) == false || vmnl_net_socket_system_no_inheritance(&sock) == false) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        vmnl_net_socket_system_close(&sock);
        return false;
    }
    (void) vmnl_net_address_from_sockaddr(&storage, address);
    *peer = (struct VmnlNetSocketSystem) {.handle = sock.handle, .family = storage.ss_family, .type = netsock->type};
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
