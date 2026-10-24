#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_dynamic_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    if (capacity == 0) {
        vmnl_net_allocator.deallocate(((struct VmnlNetBufferContiguous *) buffer)->pointer);
        buffer->capacity = 0;
        ((struct VmnlNetBufferContiguous *) buffer)->offset = 0;
        ((struct VmnlNetBufferContiguous *) buffer)->pointer = NULL;
        vmnl_net_error_set(error, VMNL_NET_SUCCESS);
        return true;
    }
    return vmnl_net_buffer_ring_resize(buffer, capacity, error);
}
