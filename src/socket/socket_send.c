#include "socket/socket_internal.h"

#include <stddef.h>

size_t vmnl_net_socket_send(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return vmnl_net_socket_send_to(netsock, NULL, views, nviews, error);
}
