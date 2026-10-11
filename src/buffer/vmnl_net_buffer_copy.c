#include "buffer/buffer_internal.h"

#include <string.h>

void vmnl_net_buffer_copy(const VmnlNetBuffer *buffer, void *data, size_t size)
{
    VmnlNetBufferView views[2];
    size_t view_size;

    if (size == 0) {
        return;
    }
    vmnl_net_buffer_view(buffer, views, NULL);
    view_size = (size > views[0].size) ? views[0].size : size;
    memcpy(data, views[0].data, view_size);
    memcpy((uint8_t *) data + view_size, views[1].data, size - view_size);
}
