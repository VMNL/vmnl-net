#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_prepare(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    if (nbytes == 0) {
        *nviews = 0;
        vmnl_net_error_set(error, VMNL_NET_SUCCESS);
        return true;
    }
    return buffer->ops->prepare(buffer, nbytes, views, nviews, error);
}
