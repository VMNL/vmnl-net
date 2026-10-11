#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <string.h>

/**
 * \brief Gets free space in a ring buffer.
 */
static bool vmnl_net_buffer_claim_ring(VmnlNetBuffer *buffer, size_t size, VmnlNetBufferView views[2], VmnlNetError *error)
{
    size_t free_size = buffer->capacity - buffer->size;
    size_t end = buffer->start + buffer->size;
    size_t view_size;

    if (size > free_size) {
        vmnl_net_error_set(error, VMNL_NET_ENOBUFS);
        return false;
    }
    if (end >= buffer->capacity) {
        end -= buffer->capacity;
    }
    view_size = (free_size > buffer->capacity - end) ? buffer->capacity - end : free_size;
    views[0] = (VmnlNetBufferView) {.data = buffer->data + end, .size = view_size};
    views[1] = (VmnlNetBufferView) {.data = buffer->data, .size = free_size - view_size};
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}

/**
 * \brief Gets free space in a dynamic buffer, growing it if needed.
 */
static bool vmnl_net_buffer_claim_dynamic(VmnlNetContext *context, VmnlNetBuffer *buffer, size_t size, VmnlNetBufferView views[2], VmnlNetError *error)
{
    size_t new_capacity = buffer->capacity + buffer->capacity / 2;

    if (new_capacity < buffer->size + size) {
        new_capacity = buffer->size + size;
    }
    if (size > buffer->capacity - buffer->size && vmnl_net_buffer_resize(context, buffer, new_capacity, error) == false) {
        return false;
    }
    if (size > buffer->capacity - buffer->start - buffer->size) {
        memmove(buffer->data, buffer->data + buffer->start, buffer->size);
        buffer->start = 0;
    }
    views[0] = (VmnlNetBufferView) {.data = buffer->data + buffer->start + buffer->size, .size = buffer->capacity - buffer->start - buffer->size};
    views[1] = (VmnlNetBufferView) {.data = buffer->data, .size = 0};
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}

bool vmnl_net_buffer_claim(VmnlNetContext *context, VmnlNetBuffer *buffer, size_t size, VmnlNetBufferView views[2], VmnlNetError *error)
{
    if (buffer->type == VMNL_NET_BUFFER_TYPE_RING) {
        return vmnl_net_buffer_claim_ring(buffer, size, views, error);
    }
    return vmnl_net_buffer_claim_dynamic(context, buffer, size, views, error);
}
