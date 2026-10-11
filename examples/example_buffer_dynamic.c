#include <vmnl/net.h>

int main(void)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(context, 0, NULL);
    //! [dynamic]
    char text[5];

    vmnl_net_buffer_write(context, buffer, "Hello", 5, NULL);
    vmnl_net_buffer_write(context, buffer, "World", 5, NULL); // Grows the buffer
    vmnl_net_buffer_read(buffer, text, 5, NULL); // "Hello", removed from the buffer
    vmnl_net_buffer_peek(buffer, text, 5, NULL); // "World", kept in the buffer
    vmnl_net_buffer_consume(buffer, 5, NULL); // Drops "World"
    vmnl_net_buffer_reserve(context, buffer, vmnl_net_buffer_capacity(buffer) * 2, NULL); // Doubles the room in one step
    vmnl_net_buffer_clear(buffer); // Drops every byte, keeps the room
    //! [dynamic]
    vmnl_net_buffer_destroy(context, buffer);
    vmnl_net_context_destroy(context);
    return 0;
}
