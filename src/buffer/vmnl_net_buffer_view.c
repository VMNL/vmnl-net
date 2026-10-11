#include "buffer/buffer_internal.h"

void vmnl_net_buffer_view(const VmnlNetBuffer *buffer, VmnlNetBufferView views[2])
{
    size_t view_size = buffer->capacity - buffer->start;

    if (view_size > buffer->size) {
        view_size = buffer->size;
    }
    views[0] = (VmnlNetBufferView) {.data = buffer->data + buffer->start, .size = view_size};
    views[1] = (VmnlNetBufferView) {.data = buffer->data, .size = buffer->size - view_size};
}
