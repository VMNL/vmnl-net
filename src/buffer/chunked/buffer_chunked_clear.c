#include "buffer/buffer_internal.h"

void vmnl_net_buffer_chunked_clear(VmnlNetBuffer *buffer)
{
    struct VmnlNetBufferChunk *chunk;

    buffer->size = 0;
    VMNL_NET_LIST_FOREACH(chunk, &((struct VmnlNetBufferChunked *) buffer)->chunks, link)
    {
        chunk->offset = 0;
        chunk->size = 0;
    }
    ((struct VmnlNetBufferChunked *) buffer)->last_used = NULL;
}
