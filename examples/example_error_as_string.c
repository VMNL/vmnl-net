#include <vmnl/net/buffer.h>

#include <stdio.h>

//! [as_string]
#define HEADER_SIZE 64

static void remove_header(VmnlNetBuffer *buffer)
{
    VmnlNetError error;

    // Fails while the buffer holds fewer than 64 bytes.
    if (vmnl_net_buffer_consume(buffer, HEADER_SIZE, &error) == false) {
        printf("%s\n", vmnl_net_error_as_string(error)); // Invalid argument
    }
    // Fails too, without writing a code.
    vmnl_net_buffer_consume(buffer, HEADER_SIZE, NULL); // == false
}
//! [as_string]

int main(void)
{
    VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_FLAT, 16, NULL);

    if (buffer == NULL) {
        return 1;
    }
    remove_header(buffer);
    vmnl_net_buffer_destroy(buffer);
    return 0;
}
