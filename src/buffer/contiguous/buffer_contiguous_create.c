#include "buffer/buffer_internal.h"
#include "error/error_internal.h"
#include "allocator/allocator_internal.h"

VmnlNetBuffer *vmnl_net_buffer_contiguous_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error)
{
    struct VmnlNetBufferContiguous *buffer = vmnl_net_allocator.allocate(sizeof *buffer);

    if (buffer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *buffer = (struct VmnlNetBufferContiguous) {
        .base = (VmnlNetBuffer) {.ops = ops, .capacity = capacity},
        .pointer = capacity > 0 ? vmnl_net_allocator.allocate(capacity) : NULL,
    };
    if (capacity > 0 && buffer->pointer == NULL) {
        vmnl_net_allocator.deallocate(buffer);
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return &buffer->base;
}
