#include "allocator/allocator_internal.h"
#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <stdint.h>
#include <string.h>

bool vmnl_net_buffer_ring_resize(VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error)
{
    uint8_t *pointer = vmnl_net_allocator.allocate(capacity);
    VmnlNetBufferView views[2] = {0};
    size_t nviews = 2;
    size_t copied_bytes = 0;

    if (pointer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    while (buffer->ops->data(buffer, copied_bytes, views, &nviews) > 0) {
        for (size_t it = 0; it < nviews; it++) {
            memcpy(pointer + copied_bytes, views[it].data, views[it].size);
            copied_bytes += views[it].size;
        }
        nviews = 2;
    }
    vmnl_net_allocator.deallocate(((struct VmnlNetBufferContiguous *) buffer)->pointer);
    buffer->capacity = capacity;
    ((struct VmnlNetBufferContiguous *) buffer)->offset = 0;
    ((struct VmnlNetBufferContiguous *) buffer)->pointer = pointer;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
