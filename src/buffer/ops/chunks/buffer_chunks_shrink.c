#include "buffer/buffer_internal.h"
#include "allocator/allocator_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunks_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error)
{
    struct VmnlNetBufferChunk *chunk;
    size_t total_capacity = 0;
    size_t total_chunks_deallocated = 0;

    if (capacity == 0 || capacity > buffer->capacity) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    VMNL_NET_LIST_FOREACH(chunk, &((struct VmnlNetBufferChunks *) buffer)->chunks, link) {
        total_capacity += ((struct VmnlNetBufferChunks *) buffer)->chunk_size;
        if (total_capacity <= capacity) {
            continue;
        }
        vmnl_net_list_remove(&chunk->link);
        vmnl_net_allocator.deallocate(chunk->pointer);
        vmnl_net_allocator.deallocate(chunk);
        total_chunks_deallocated++;
    }
    vmnl_net_error_set(error, (total_chunks_deallocated == 0) ? VMNL_NET_ENOMEM : VMNL_NET_SUCCESS);
    return total_chunks_deallocated > 0;
}
