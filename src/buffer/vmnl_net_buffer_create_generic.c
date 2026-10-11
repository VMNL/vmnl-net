#include "buffer/buffer_internal.h"
#include "context/context_internal.h"
#include "error/error_internal.h"

VmnlNetBuffer *vmnl_net_buffer_create_generic(VmnlNetContext *context, VmnlNetBufferType type, size_t capacity, VmnlNetError *error)
{
    VmnlNetBuffer *buffer;

    buffer = context->allocator.allocate(sizeof *buffer);
    if (buffer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *buffer = (VmnlNetBuffer) {.type = type};
    if (capacity != 0 && vmnl_net_buffer_resize(context, buffer, capacity, error) == false) {
        context->allocator.deallocate(buffer);
        return NULL;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return buffer;
}
