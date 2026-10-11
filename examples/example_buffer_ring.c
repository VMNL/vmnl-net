#include <vmnl/net.h>

int main(void)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_ring(context, 8, NULL);
    //! [ring]
    VmnlNetError error;
    char text[6];

    vmnl_net_buffer_write(context, buffer, "abcdef", 6, NULL);
    vmnl_net_buffer_write(context, buffer, "ghi", 3, &error); // Fails with VMNL_NET_ENOBUFS, a ring buffer never grows
    vmnl_net_buffer_read(buffer, text, 4, NULL); // Frees 4 bytes at the front
    vmnl_net_buffer_write(context, buffer, "ghij", 4, NULL); // Goes around the end of the storage
    vmnl_net_buffer_read(buffer, text, 6, NULL); // "efghij", in order
    //! [ring]
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
    return 0;
}
