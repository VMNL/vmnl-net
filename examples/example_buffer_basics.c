#include <vmnl/net/buffer.h>

#include <string.h>
#include <stdio.h>

int main(void)
{
    //! [create_buffer]
    VmnlNetError error;
    
    // We already know the size of the payload, so VMNL_NET_BUFFER_FLAT is the best choice here.
    VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_FLAT, 42, &error);

    if (buffer == NULL) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        return 1;
    }
    //! [create_buffer]

    //! [write_buffer]
    const char payload[] = "Hello, World!\n";
    size_t payload_size = sizeof payload - 1;
    
    // A flat buffer only requires one view.
    VmnlNetBufferView view;
    size_t nviews = 1;

    // Making room for payload's size (14, without terminating '\0') bytes and retrieving a view to write to.
    if (vmnl_net_buffer_prepare(buffer, payload_size, &view, &nviews, &error) == false) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_buffer_destroy(buffer);
        return 1;
    }

    // Writing to the buffer through the view. recv(socket, view.data, view.size, 0) also works!
    memcpy(view.data, payload, payload_size);

    // Committing the written bytes, making them readable.
    if (vmnl_net_buffer_commit(buffer, payload_size, &error) == false) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_buffer_destroy(buffer);
        return 1;
    }
    //! [write_buffer]

    //! [read_buffer]
    // vmnl_net_buffer_prepare() may have updated the value, so we set it back to its original value.
    nviews = 1;
    
    // We want to start at the beginning of the stored bytes, so a 0 offset is required.
    vmnl_net_buffer_data(buffer, 0, &view, &nviews);

    // Handles the bytes, like a packet sent to a peer.
    fwrite(view.data, 1, view.size, stdout);

    // We "consumed" our bytes so we drop them.
    if (vmnl_net_buffer_consume(buffer, view.size, &error) == false) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_buffer_destroy(buffer);
        return 1;
    }
    //! [read_buffer]

    //! [destroy_buffer]
    // Once done, we can destroy our buffer to release its storage.
    vmnl_net_buffer_destroy(buffer);
    //! [destroy_buffer]

    return 0;
}
