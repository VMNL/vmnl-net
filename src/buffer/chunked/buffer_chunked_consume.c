#include "buffer/buffer_internal.h"

void vmnl_net_buffer_chunked_consume(VmnlNetBuffer *buffer, const size_t nbytes)
{
    struct VmnlNetBufferChunked *chunked_buffer = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *chunk;
    size_t nbytes_to_consume = nbytes;
    size_t chunk_consumed_bytes;

    while (nbytes_to_consume > 0) {
        chunk = VMNL_NET_LIST_ENTRY(chunked_buffer->chunks.next, struct VmnlNetBufferChunk, link);
        chunk_consumed_bytes = (nbytes_to_consume < chunk->size) ? nbytes_to_consume : chunk->size;
        nbytes_to_consume -= chunk_consumed_bytes;
        buffer->size -= chunk_consumed_bytes;
        chunk->offset += chunk_consumed_bytes;
        chunk->size -= chunk_consumed_bytes;
        if (chunk->size == 0) {
            vmnl_net_list_remove(&chunk->link);
            vmnl_net_list_push_back(&chunked_buffer->chunks, &chunk->link);
            chunk->offset = 0;
        }
    }
    if (buffer->size == 0) {
        chunked_buffer->commit_tail = NULL;
    }
}
