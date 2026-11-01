#include <vmnl/net/buffer.h>
#include <vmnl/net/error.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

//! [read_write]
static bool read_write(void)
{
    VmnlNetError error = VMNL_NET_SUCCESS;
    VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_DYNAMIC, 0, &error);
    VmnlNetBufferView view;
    size_t nviews = 1;
    size_t nbytes = 0;
    bool success = false;

    if (buffer == NULL) {
        printf("%s\n", vmnl_net_error_as_string(error)); // Cannot allocate memory
        return false;
    }
    // Writing: asks for 5 free bytes, writes into them, then stores them
    if (vmnl_net_buffer_prepare(buffer, 5, &view, &nviews, &error) == false) {
        printf("%s\n", vmnl_net_error_as_string(error)); // Cannot allocate memory
        vmnl_net_buffer_destroy(buffer);
        return false;
    }
    memcpy(view.data, "hello", 5); // written in the free space
    if (vmnl_net_buffer_commit(buffer, 5, &error) == false) {
        printf("%s\n", vmnl_net_error_as_string(error));
        vmnl_net_buffer_destroy(buffer);
        return false;
    }
    // Reading: describes the stored bytes, which stay stored
    nviews = 1;
    nbytes = vmnl_net_buffer_data(buffer, 0, &view, &nviews); // view: "hello"
    if (nbytes == 5 && memcmp(view.data, "hello", 5) == 0) {
        fwrite(view.data, 1, view.size, stdout);
        printf("\n");
        // Drops the bytes that were handled
        success = vmnl_net_buffer_consume(buffer, nbytes, &error) == true && vmnl_net_buffer_is_empty(buffer) == true;
    }
    vmnl_net_buffer_destroy(buffer);
    return success;
}
//! [read_write]

int main(void)
{
    return (read_write() == true) ? 0 : 1;
}
