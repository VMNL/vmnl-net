#include "buffer/buffer_internal.h"
#include "allocator/allocator_internal.h"
#include "error/error_internal.h"

static bool create_new_memory_chunk(struct VmnlNetListLink *chunks, const size_t chunk_capacity)
{
    struct VmnlNetBufferChunk *chunk = vmnl_net_allocator.allocate(chunk_capacity);

    if (chunk == NULL) {
        return false;
    }
    *chunk = (struct VmnlNetBufferChunk) {.pointer = vmnl_net_allocator.allocate(sizeof *chunk)};
    if (chunk->pointer == NULL) {
        vmnl_net_allocator.deallocate(chunk);
        return false;
    }
    vmnl_net_list_push_back(chunks, &chunk->link);
    return true;
}

bool vmnl_net_buffer_chunks_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    const size_t chunk_size = ((const struct VmnlNetBufferChunks *) buffer)->chunk_size;
    size_t buffer_capacity = buffer->capacity;
    struct VmnlNetListLink chunks;

    vmnl_net_list_init(&chunks);
    while (buffer_capacity < capacity) {
        if (create_new_memory_chunk(&chunks, chunk_size) == true) {
            vmnl_net_buffer_chunks_destroy_list(&((struct VmnlNetBufferChunks *) buffer)->chunks);
            vmnl_net_error_set(error, VMNL_NET_ENOMEM);
            return false;
        }
        buffer_capacity += chunk_size;
    }
    buffer->capacity = buffer_capacity;
    vmnl_net_list_merge(&((struct VmnlNetBufferChunks *) buffer)->chunks, &chunks);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
