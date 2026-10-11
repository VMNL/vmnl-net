#include <vmnl/net.h>

#include <stdio.h>

int main(void)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(context, 0, NULL);
    // VmnlNetBuffer *buffer = vmnl_net_buffer_create_ring(context, 128, NULL);
    char header;
    char body[4];

    vmnl_net_buffer_reserve(context, buffer, 1024, NULL); // Fails with VMNL_NET_ENOBUFS on a ring buffer, which cannot grow
    vmnl_net_buffer_write(context, buffer, "hello!", 6, NULL); // Fails with VMNL_NET_ENOBUFS on a FULL ring buffer
    printf("%zu of %zu bytes used\n", vmnl_net_buffer_size(buffer), vmnl_net_buffer_capacity(buffer));
    vmnl_net_buffer_peek(buffer, &header, 1, NULL); // 'h', kept in the buffer
    vmnl_net_buffer_consume(buffer, 1, NULL); // drops 'h'
    vmnl_net_buffer_read(buffer, body, 4, NULL); // "ello", removed from the buffer
    vmnl_net_buffer_clear(buffer); // drops '!'
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
    return 0;
}
