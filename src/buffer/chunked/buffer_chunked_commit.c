#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunked_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
{
    struct VmnlNetBufferChunked *chunked = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *first = VMNL_NET_LIST_FRONT(&chunked->chunks, struct VmnlNetBufferChunk, link);
    struct VmnlNetBufferChunk *chunk = (chunked->last_used != NULL) ? chunked->last_used : first;
    size_t free_space = buffer->capacity - buffer->size;
    size_t bytes = nbytes;
    size_t room;
    size_t committed;

    if (first != NULL) {
        free_space -= first->offset;
    }
    if (nbytes > free_space) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    while (bytes > 0) {
        if (chunk->offset + chunk->size == VMNL_NET_BUFFER_CHUNK_SIZE) {
            chunk = VMNL_NET_LIST_ENTRY(chunk->link.next, struct VmnlNetBufferChunk, link);
        }
        room = VMNL_NET_BUFFER_CHUNK_SIZE - chunk->offset - chunk->size;
        committed = (bytes < room) ? bytes : room;
        chunk->size += committed;
        bytes -= committed;
        chunked->last_used = chunk;
    }
    buffer->size += nbytes;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
