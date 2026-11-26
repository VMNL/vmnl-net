#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <string.h>

bool vmnl_net_buffer_ring_resize(VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error)
{
    struct VmnlNetBufferContiguous *contiguous_buffer = (struct VmnlNetBufferContiguous *) buffer;
    uint8_t *pointer = vmnl_net_allocator.allocate(capacity);
    struct VmnlNetBufferSpan span;
    struct VmnlNetBufferSpan other;

    if (pointer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    if (buffer->size > 0) {
        span = vmnl_net_buffer_ring_spans(buffer, contiguous_buffer->offset, buffer->size, &other);
        memcpy(pointer, span.data, span.size);
        memcpy(pointer + span.size, other.data, other.size);
    }
    vmnl_net_allocator.deallocate(contiguous_buffer->pointer);
    buffer->capacity = capacity;
    contiguous_buffer->offset = 0;
    contiguous_buffer->pointer = pointer;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
