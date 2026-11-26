#include "address/address_internal.h"
#include "error/error_internal.h"
#include "socket/socket_internal.h"

#include <stdbool.h>

bool vmnl_net_socket_accept(struct VmnlNetSocket *netsock, struct VmnlNetSocket *peer, VmnlNetAddress *address, VmnlNetError *error)
{
    struct sockaddr_storage storage;
    socklen_t socklen = sizeof storage;
    struct VmnlNetSocket sock = {.handle = accept(netsock->handle, (struct sockaddr *) &storage, &socklen)};

    if (sock.handle == VMNL_NET_SOCKET_INVALID) {
        vmnl_net_error_set(error, vmnl_net_error_from_last_system());
        return false;
    }
    if (vmnl_net_socket_non_blocking(&sock, error) == false || vmnl_net_socket_no_inheritance(&sock, error) == false) {
        vmnl_net_socket_close(&sock);
        return false;
    }
    (void) vmnl_net_address_from_sockaddr(&storage, address);
    peer->handle = sock.handle;
    peer->family = storage.ss_family;
    peer->type = netsock->type;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
