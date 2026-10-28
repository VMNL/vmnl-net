#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <string.h>

bool vmnl_net_buffer_ring_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    uint8_t *pointer = vmnl_net_allocator.allocate(capacity);
    VmnlNetBufferView views[2] = {0};

    if (pointer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    if (buffer->ops->data(buffer, 0, views, &(size_t) {2}) > 0) {
        memcpy(pointer, views[0].data, views[0].size);
        if (views[1].data != NULL) {
            memcpy(pointer + views[0].size, views[1].data, views[1].size);
        }
    }
    vmnl_net_allocator.deallocate(((struct VmnlNetBufferContiguous *) buffer)->pointer);
    buffer->capacity = capacity;
    ((struct VmnlNetBufferContiguous *) buffer)->offset = 0;
    ((struct VmnlNetBufferContiguous *) buffer)->pointer = pointer;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
