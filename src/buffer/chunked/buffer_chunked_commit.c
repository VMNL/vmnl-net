#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunked_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
{
    struct VmnlNetBufferChunked *chunked_buffer = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *front_chunk = VMNL_NET_LIST_FRONT(&chunked_buffer->chunks, struct VmnlNetBufferChunk, link);
    struct VmnlNetBufferChunk *chunk = (chunked_buffer->commit_tail == NULL) ? front_chunk : chunked_buffer->commit_tail;
    size_t chunk_available_space;
    size_t chunk_committed_bytes;

    if (nbytes > buffer->capacity - buffer->size - ((front_chunk == NULL) ? 0 : front_chunk->offset)) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    for (size_t nbytes_to_commit = nbytes; nbytes_to_commit > 0; chunk = VMNL_NET_LIST_ENTRY(chunk->link.next, struct VmnlNetBufferChunk, link)) {
        chunk_available_space = VMNL_NET_BUFFER_CHUNK_SIZE - chunk->offset - chunk->size;
        chunk_committed_bytes = (nbytes_to_commit < chunk_available_space) ? nbytes_to_commit : chunk_available_space;
        nbytes_to_commit -= chunk_committed_bytes;
        chunk->size += chunk_committed_bytes;
        buffer->size += chunk_committed_bytes;
        chunked_buffer->commit_tail = chunk;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
