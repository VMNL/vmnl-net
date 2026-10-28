#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_flat_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    if (nbytes > buffer->capacity - buffer->size) {
        vmnl_net_error_set(error, VMNL_NET_ENOBUFS);
        return false;
    }
    return vmnl_net_buffer_dynamic_prepare(buffer, nbytes, views, nviews, error);
}
