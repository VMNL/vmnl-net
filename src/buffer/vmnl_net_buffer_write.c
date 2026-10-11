#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <string.h>

bool vmnl_net_buffer_write(VmnlNetContext *context, VmnlNetBuffer *buffer, const void *data, size_t size, VmnlNetError *error)
{
    VmnlNetBufferView views[2];
    size_t view_size;

    if (context == NULL || buffer == NULL || (data == NULL && size != 0)) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (size == 0) {
        vmnl_net_error_set(error, VMNL_NET_SUCCESS);
        return true;
    }
    if (vmnl_net_buffer_claim(context, buffer, size, views, error) == false) {
        return false;
    }
    view_size = (size > views[0].size) ? views[0].size : size;
    memcpy(views[0].data, data, view_size);
    memcpy(views[1].data, (const uint8_t *) data + view_size, size - view_size);
    vmnl_net_buffer_commit(buffer, size, NULL);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
