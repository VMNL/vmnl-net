#include "buffer/buffer_internal.h"

#include <string.h>

size_t vmnl_net_buffer_flat_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    const struct VmnlNetBufferContiguous *contiguous_buffer = (const struct VmnlNetBufferContiguous *) buffer;

    if (offset >= buffer->size || *nviews == 0) {
        *nviews = 0;
        return 0;
    }
    views[0].data = contiguous_buffer->pointer + contiguous_buffer->offset + offset;
    views[0].size = buffer->size - offset;
    *nviews = 1;
    return views[0].size;
}
