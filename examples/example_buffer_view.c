#include <vmnl/net.h>

#include <string.h>

int main(void)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(context, 0, NULL);
    //! [view]
    VmnlNetBufferView views[2];

    vmnl_net_buffer_claim(context, buffer, 5, views, NULL); // Free space to write into directly, like recv() would
    memcpy(views[0].data, "Hello", 5);
    vmnl_net_buffer_commit(buffer, 5, NULL); // The 5 bytes now hold data
    vmnl_net_buffer_view(buffer, views, NULL); // views[0] holds "Hello", views[1] is empty unless a ring wraps around
    vmnl_net_buffer_consume(buffer, vmnl_net_buffer_size(buffer), NULL); // Drops every stored byte
    //! [view]
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
    return 0;
}
