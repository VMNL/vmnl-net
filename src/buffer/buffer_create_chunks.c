#include "buffer/buffer_internal.h"
#include "allocator/allocator_internal.h"
#include "error/error_internal.h"

static const struct VmnlNetBufferOps vmnl_buffer_chunks_ops = {
    .clear      = vmnl_net_buffer_chunks_clear,
    .resize     = vmnl_net_buffer_chunks_resize,
    .shrink     = vmnl_net_buffer_chunks_shrink,
    .data       = vmnl_net_buffer_chunks_data,
    .consume    = vmnl_net_buffer_chunks_consume,
    .prepare    = vmnl_net_buffer_chunks_prepare,
    .commit     = vmnl_net_buffer_chunks_commit,
    .destroy    = vmnl_net_buffer_chunks_destroy,
};

VmnlNetBuffer *vmnl_net_buffer_create_chunks(const size_t nchunks, const size_t chunk_size, VmnlNetError *error)
{
    struct VmnlNetBufferChunks *buffer;

    if (chunk_size == 0) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;       
    }
    buffer = vmnl_net_allocator.allocate(sizeof *buffer);
    if (buffer == NULL) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *buffer = (struct VmnlNetBufferChunks) {
        .base = {.ops = &vmnl_buffer_chunks_ops, .capacity = nchunks * chunk_size},
        .chunk_size = chunk_size,
    };
    vmnl_net_list_init(&buffer->chunks);
    if (buffer->base.ops->resize(&buffer->base, nchunks * chunk_size, error) == false) {
        vmnl_net_allocator.deallocate(buffer);
        return NULL;
    }
    return &buffer->base;
}
