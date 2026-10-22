#include "buffer/buffer_internal.h"

void vmnl_net_buffer_contiguous_clear(VmnlNetBuffer *buffer)
{
    buffer->size = 0;
    ((struct VmnlNetBufferContiguous *) buffer)->offset = 0;
}
