#include <vmnl/net.h>

#include <stdio.h>

int main(void)
{
    VmnlNetError error;
    VmnlNetContext *context = vmnl_net_context_create(&error);
    VmnlNetBuffer *buffer;

    // The return value signals a failure and error tells why, it holds VMNL_NET_SUCCESS otherwise
    if (context == NULL) {
        fprintf(stderr, "%s\n", vmnl_net_error_string(error));
        return 1;
    }
    // Every call overwrites error, read it before passing it to another one
    buffer = vmnl_net_buffer_create_ring(context, 0, &error);
    if (buffer == NULL && error == VMNL_NET_EINVAL) {
        // Pass NULL when the reason does not matter
        buffer = vmnl_net_buffer_create_ring(context, 1024, NULL);
    }
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
    return 0;
}
