#include "buffer/buffer_internal.h"
#include "context/context_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_resize(VmnlNetContext *context, VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error)
{
    uint8_t *data = context->allocator.reallocate(buffer->data, capacity);

    if (data == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    buffer->data = data;
    buffer->capacity = capacity;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
