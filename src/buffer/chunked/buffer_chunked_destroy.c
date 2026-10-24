#include "buffer/buffer_internal.h"

void vmnl_net_buffer_chunked_destroy(VmnlNetBuffer *buffer)
{
    vmnl_net_buffer_chunked_destroy_internal_chunks(&((struct VmnlNetBufferChunked *) buffer)->chunks);
    vmnl_net_allocator.deallocate(buffer);
}
