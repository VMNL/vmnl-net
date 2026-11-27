#include <vmnl/net/clock.h>
#include <vmnl/net/context.h>
#include <vmnl/net/socket/datagram.h>

#include <stdio.h>

int main(void)
{
    //! [create_context]
    VmnlNetError error;
    // On Windows, the context starts the sockets, so it comes first and outlives them.
    VmnlNetContext *context = vmnl_net_context_create(&error);

    if (context == NULL) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        return 1;
    }
    //! [create_context]

    //! [create_socket]
    VmnlNetAddress address;

    // VMNL_NET_ADDRESS_ANY_PORT lets the system choose a free port.
    vmnl_net_address_make("127.0.0.1", VMNL_NET_ADDRESS_ANY_PORT, &address, NULL);

    // The type picks UDP, and the address picks IPv4 or IPv6.
    VmnlNetSocketDatagram *receiver = vmnl_net_socket_datagram_create(VMNL_NET_SOCKET_DATAGRAM_UDP, vmnl_net_address_version(&address), &error);

    if (receiver == NULL) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_context_destroy(context);
        return 1;
    }

    // Binding gives the receiver an address. We then retrieve the port the system chose.
    if (vmnl_net_socket_datagram_bind(receiver, &address, &error) == false || vmnl_net_socket_datagram_local_address(receiver, &address, &error) == false) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_socket_datagram_destroy(receiver);
        vmnl_net_context_destroy(context);
        return 1;
    }
    //! [create_socket]

    //! [send_datagram]
    VmnlNetBufferView view = {.data = (uint8_t *) "hello", .size = 5};

    // An unbound socket gets a free port at its first send.
    VmnlNetSocketDatagram *sender = vmnl_net_socket_datagram_create(VMNL_NET_SOCKET_DATAGRAM_UDP, vmnl_net_address_version(&address), &error);

    // A datagram is sent whole or not at all.
    if (sender == NULL || vmnl_net_socket_datagram_send_to(sender, &address, &view, 1, &error) == 0) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_socket_datagram_destroy(sender);
        vmnl_net_socket_datagram_destroy(receiver);
        vmnl_net_context_destroy(context);
        return 1;
    }
    //! [send_datagram]

    //! [receive_datagram]
    uint8_t bytes[512];
    VmnlNetAddress peer;

    view = (VmnlNetBufferView) {.data = bytes, .size = 512};

    // The socket never blocks, so we wait for the datagram, for one second at most.
    if (vmnl_net_socket_datagram_wait_for(receiver, VMNL_NET_SOCKET_READABLE, VMNL_NET_NANOSECONDS_PER_SECOND, &error) != VMNL_NET_SOCKET_READABLE) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_socket_datagram_destroy(sender);
        vmnl_net_socket_datagram_destroy(receiver);
        vmnl_net_context_destroy(context);
        return 1;
    }

    size_t nbytes = vmnl_net_socket_datagram_receive_from(receiver, &view, 1, &peer, &error);

    if (error != VMNL_NET_SUCCESS) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_socket_datagram_destroy(sender);
        vmnl_net_socket_datagram_destroy(receiver);
        vmnl_net_context_destroy(context);
        return 1;
    }
    printf("%.*s\n", (int) nbytes, (const char *) bytes); // hello
    //! [receive_datagram]

    //! [destroy_socket]
    // The sockets go before the context they need.
    vmnl_net_socket_datagram_destroy(sender);
    vmnl_net_socket_datagram_destroy(receiver);
    vmnl_net_context_destroy(context);
    //! [destroy_socket]

    return 0;
}
