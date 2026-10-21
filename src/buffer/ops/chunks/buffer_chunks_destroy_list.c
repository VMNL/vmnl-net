#include "buffer/buffer_internal.h"
#include "allocator/allocator_internal.h"

void vmnl_net_buffer_chunks_destroy_list(struct VmnlNetListLink *chunks)
{
    struct VmnlNetBufferChunk *chunk;

    VMNL_NET_LIST_FOREACH(chunk, chunks, link) {
        vmnl_net_allocator.deallocate(chunk->pointer);
        vmnl_net_allocator.deallocate(chunk);
    }
}
