#include "buffer/buffer_internal.h"
#include "allocator/allocator_internal.h"

void vmnl_net_buffer_chunks_destroy(VmnlNetBuffer *buffer)
{
    vmnl_net_buffer_chunks_destroy_list(&((struct VmnlNetBufferChunks *) buffer)->chunks);
    vmnl_net_allocator.deallocate(buffer);
}
