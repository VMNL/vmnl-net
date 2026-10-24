#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunked_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    struct VmnlNetBufferChunked *chunked_buffer = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *chunk = VMNL_NET_LIST_BACK(&chunked_buffer->chunks, struct VmnlNetBufferChunk, link);

    while (buffer->capacity - capacity >= VMNL_NET_BUFFER_CHUNK_SIZE && chunk->size == 0) {
        vmnl_net_list_remove(&chunk->link);
        vmnl_net_allocator.deallocate(chunk);
        buffer->capacity -= VMNL_NET_BUFFER_CHUNK_SIZE;
        chunk = VMNL_NET_LIST_BACK(&chunked_buffer->chunks, struct VmnlNetBufferChunk, link);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
