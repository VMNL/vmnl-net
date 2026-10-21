#include "buffer/buffer_internal.h"

void vmnl_net_buffer_chunks_clear(VmnlNetBuffer *buffer)
{
    struct VmnlNetBufferChunk *chunk;

    VMNL_NET_LIST_FOREACH(chunk, &((struct VmnlNetBufferChunks *) buffer)->chunks, link) {
        chunk->size = 0;
        chunk->offset = 0;
    }
    buffer->size = 0;
}
