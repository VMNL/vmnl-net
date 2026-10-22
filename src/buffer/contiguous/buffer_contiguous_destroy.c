#include "buffer/buffer_internal.h"
#include "allocator/allocator_internal.h"

void vmnl_net_buffer_contiguous_destroy(VmnlNetBuffer *buffer)
{
    vmnl_net_allocator.deallocate(((struct VmnlNetBufferContiguous *) buffer)->pointer);
    vmnl_net_allocator.deallocate(buffer);
}
