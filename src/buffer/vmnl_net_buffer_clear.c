#include "buffer/buffer_internal.h"

void vmnl_net_buffer_clear(VmnlNetBuffer *buffer)
{
    if (buffer == NULL) {
        return;
    }
    buffer->size = 0;
    buffer->start = 0;
}
