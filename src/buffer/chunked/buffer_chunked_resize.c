#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunked_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    struct VmnlNetBufferChunk *chunk;
    struct VmnlNetListLink chunks;
    size_t added_capacity = 0;

    vmnl_net_list_init(&chunks);
    while (buffer->capacity + added_capacity < capacity) {
        chunk = vmnl_net_allocator.allocate(sizeof *chunk + VMNL_NET_BUFFER_CHUNK_SIZE);
        if (chunk == NULL) {
            vmnl_net_buffer_chunked_destroy_internal_chunks(&chunks);
            vmnl_net_error_set(error, VMNL_NET_ENOMEM);
            return false;
        }
        chunk->offset = 0;
        chunk->size = 0;
        vmnl_net_list_push_back(&chunks, &chunk->link);
        added_capacity += VMNL_NET_BUFFER_CHUNK_SIZE;
    }
    buffer->capacity += added_capacity;
    vmnl_net_list_splice(&((struct VmnlNetBufferChunked *) buffer)->chunks, &chunks);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
