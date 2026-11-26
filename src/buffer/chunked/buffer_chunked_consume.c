#include "buffer/buffer_internal.h"

void vmnl_net_buffer_chunked_consume(VmnlNetBuffer *buffer, size_t nbytes)
{
    struct VmnlNetBufferChunked *chunked_buffer = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *chunk;
    size_t chunk_consumed_bytes;

    while (nbytes > 0) {
        chunk = VMNL_NET_LIST_FRONT(&chunked_buffer->chunks, struct VmnlNetBufferChunk, link);
        chunk_consumed_bytes = (nbytes < chunk->size) ? nbytes : chunk->size;
        nbytes -= chunk_consumed_bytes;
        buffer->size -= chunk_consumed_bytes;
        chunk->offset += chunk_consumed_bytes;
        chunk->size -= chunk_consumed_bytes;
        if (chunk->size == 0) {
            chunk->offset = 0;
            vmnl_net_list_rotate_left(&chunked_buffer->chunks);
        }
    }
    if (buffer->size == 0) {
        chunked_buffer->commit_tail = NULL;
    }
}
