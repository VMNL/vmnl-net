#include "buffer/buffer_internal.h"

size_t vmnl_net_buffer_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    return buffer->ops->data(buffer, offset, views, nviews);
}
