#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

VmnlNetBuffer *vmnl_net_buffer_chunked_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error)
{
    struct VmnlNetBufferChunked *buffer = vmnl_net_allocator.allocate(sizeof *buffer);

    if (buffer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *buffer = (struct VmnlNetBufferChunked) {.base = (VmnlNetBuffer) {.ops = ops}};
    vmnl_net_list_init(&buffer->chunks);
    if (capacity > 0 && vmnl_net_buffer_chunked_resize(&buffer->base, capacity, error) == false) {
        vmnl_net_allocator.deallocate(buffer);
        return NULL;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return &buffer->base;
}
