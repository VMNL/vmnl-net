#include "socket/socket_internal.h"

#include <stddef.h>

size_t vmnl_net_socket_receive(struct VmnlNetSocket *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    return vmnl_net_socket_receive_from(netsock, views, nviews, NULL, error);
}
