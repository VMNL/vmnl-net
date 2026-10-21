#include "buffer/buffer_internal.h"

bool vmnl_net_buffer_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    return buffer->ops->prepare(buffer, nbytes, views, nviews, error);
}
