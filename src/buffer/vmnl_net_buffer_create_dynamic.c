#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

VmnlNetBuffer *vmnl_net_buffer_create_dynamic(VmnlNetContext *context, size_t capacity, VmnlNetError *error)
{
    if (context == NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return NULL;
    }
    return vmnl_net_buffer_create_generic(context, VMNL_NET_BUFFER_TYPE_DYNAMIC, capacity, error);
}
