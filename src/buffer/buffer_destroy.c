#include "buffer/buffer_internal.h"

void vmnl_net_buffer_destroy(VmnlNetBuffer *buffer)
{
    if (buffer == NULL) {
        return;
    }
    buffer->ops->destroy(buffer);
}
