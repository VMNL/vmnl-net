#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

static size_t vmnl_net_buffer_chunked_count_views_to_prepare(const struct VmnlNetBufferChunk *chunk, size_t nbytes)
{
    size_t chunk_free_space = (chunk == NULL) ? 0 : vmnl_net_buffer_chunk_free_space(chunk);
    size_t nbytes_after_chunk = (nbytes < chunk_free_space) ? 0 : nbytes - chunk_free_space;

    return (chunk_free_space > 0) + nbytes_after_chunk / VMNL_NET_BUFFER_CHUNK_SIZE + (nbytes_after_chunk % VMNL_NET_BUFFER_CHUNK_SIZE > 0);
}

bool vmnl_net_buffer_chunked_prepare(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    struct VmnlNetBufferChunked *chunked_buffer = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *chunk;
    size_t free_space = vmnl_net_buffer_chunked_free_space(buffer);
    size_t written_views = 0;

    if (nbytes > SIZE_MAX - buffer->capacity) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    if (vmnl_net_buffer_chunked_count_views_to_prepare(vmnl_net_buffer_chunked_writing_chunk(chunked_buffer), nbytes) > *nviews) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (nbytes > free_space && vmnl_net_buffer_chunked_resize(buffer, buffer->capacity + (nbytes - free_space), error) == false) {
        return false;
    }
    chunk = vmnl_net_buffer_chunked_writing_chunk(chunked_buffer);
    VMNL_NET_LIST_FOREACH_FROM(chunk, &chunked_buffer->chunks, link)
    {
        if (written_views == *nviews) {
            break;
        }
        if (vmnl_net_buffer_chunk_free_space(chunk) == 0) {
            continue;
        }
        views[written_views].data = chunk->bytes + chunk->offset + chunk->size;
        views[written_views].size = vmnl_net_buffer_chunk_free_space(chunk);
        written_views++;
    }
    *nviews = written_views;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
