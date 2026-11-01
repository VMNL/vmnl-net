#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunked_shrink(VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error)
{
    struct VmnlNetBufferChunk *chunk;

    VMNL_NET_LIST_FOREACH_REVERSE(chunk, &((struct VmnlNetBufferChunked *) buffer)->chunks, link)
    {
        if (buffer->capacity - capacity < VMNL_NET_BUFFER_CHUNK_SIZE || chunk->size > 0) {
            break;
        }
        vmnl_net_list_remove(&chunk->link);
        vmnl_net_allocator.deallocate(chunk);
        buffer->capacity -= VMNL_NET_BUFFER_CHUNK_SIZE;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
