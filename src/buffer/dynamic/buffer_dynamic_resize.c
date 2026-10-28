#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_dynamic_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    uint8_t *pointer = vmnl_net_allocator.reallocate(((struct VmnlNetBufferContiguous *) buffer)->pointer, capacity);

    if (pointer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    buffer->capacity = capacity;
    ((struct VmnlNetBufferContiguous *) buffer)->pointer = pointer;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
