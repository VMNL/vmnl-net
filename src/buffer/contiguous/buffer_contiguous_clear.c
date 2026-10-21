#include "buffer/buffer_internal.h"

void vmnl_net_buffer_contiguous_clear(VmnlNetBuffer *buffer)
{
    ((struct VmnlNetBufferContiguous *) buffer)->offset = 0;
}
