#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <stdint.h>

static size_t count_needed_views(const struct VmnlNetBufferChunk *chunk, const size_t nbytes)
{
    const size_t chunk_free_space = (chunk == NULL) ? 0 : VMNL_NET_BUFFER_CHUNK_SIZE - chunk->offset - chunk->size;
    const size_t nbytes_after_chunk = (nbytes < chunk_free_space) ? 0 : nbytes - chunk_free_space;

    return (chunk_free_space > 0) + nbytes_after_chunk / VMNL_NET_BUFFER_CHUNK_SIZE + (nbytes_after_chunk % VMNL_NET_BUFFER_CHUNK_SIZE > 0);
}

bool vmnl_net_buffer_chunked_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error)
{
    struct VmnlNetBufferChunked *chunked_buffer = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *front_chunk = VMNL_NET_LIST_FRONT(&chunked_buffer->chunks, struct VmnlNetBufferChunk, link);
    struct VmnlNetBufferChunk *chunk = (chunked_buffer->commit_tail == NULL) ? front_chunk : chunked_buffer->commit_tail;
    const size_t free_space = buffer->capacity - buffer->size - ((front_chunk == NULL) ? 0 : front_chunk->offset);
    size_t written_views = 0;

    if (nbytes > SIZE_MAX - buffer->capacity) {
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return false;
    }
    if (count_needed_views(chunk, nbytes) > *nviews) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (nbytes > free_space && vmnl_net_buffer_chunked_resize(buffer, buffer->capacity + (nbytes - free_space), error) == false) {
        return false;
    }
    for (chunk = (chunk == NULL) ? VMNL_NET_LIST_ENTRY(chunked_buffer->chunks.next, struct VmnlNetBufferChunk, link) : chunk; written_views < *nviews && &chunk->link != &chunked_buffer->chunks; chunk = VMNL_NET_LIST_ENTRY(chunk->link.next, struct VmnlNetBufferChunk, link)) {
        if (chunk->offset + chunk->size < VMNL_NET_BUFFER_CHUNK_SIZE) {
            views[written_views].data = chunk->bytes + chunk->offset + chunk->size;
            views[written_views].size = VMNL_NET_BUFFER_CHUNK_SIZE - chunk->offset - chunk->size;
            written_views++;
        }
    }
    *nviews = written_views;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
