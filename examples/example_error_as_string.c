#include <vmnl/net/buffer.h>
#include <vmnl/net/error.h>

#include <stdbool.h>
#include <stdio.h>

//! [as_string]
static void drop_header(VmnlNetBuffer *buffer)
{
    VmnlNetError error = VMNL_NET_SUCCESS;

    // Fails while the buffer holds less than 64 bytes
    if (vmnl_net_buffer_consume(buffer, 64, &error) == false) {
        printf("%s\n", vmnl_net_error_as_string(error)); // Invalid argument
    }
    // Fails too, without writing a code
    vmnl_net_buffer_consume(buffer, 64, NULL);
}
//! [as_string]

int main(void)
{
    VmnlNetBuffer *buffer =
        vmnl_net_buffer_create(VMNL_NET_BUFFER_FLAT, 16, NULL);

    if (buffer == NULL) {
        return 1;
    }
    drop_header(buffer);
    vmnl_net_buffer_destroy(buffer);
    return 0;
}
