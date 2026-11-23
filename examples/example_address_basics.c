#include <vmnl/net/address.h>

#include <stdbool.h>
#include <stdio.h>

//! [make]
static bool make_server(VmnlNetAddress *server)
{
    VmnlNetError error = VMNL_NET_SUCCESS;

    // A host name like localhost is refused, only an IP is parsed.
    if (vmnl_net_address_make("192.0.2.1", 8080, server, &error) == false) {
        printf("%s\n", vmnl_net_error_as_string(error));
        return false;
    }
    return vmnl_net_address_is_ipv4(server); // true
}
//! [make]

//! [as_string]
static void print_peer(const VmnlNetAddress *peer)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];

    vmnl_net_address_as_string(peer, text, sizeof text, NULL);
    printf("%s port %u\n", text, (unsigned) peer->port); // 192.0.2.1 port 8080
}
//! [as_string]

//! [is_equal]
static bool is_server(const VmnlNetAddress *peer, const VmnlNetAddress *server)
{
    // Never memcmp(), as the struct has padding bytes.
    return vmnl_net_address_is_equal(peer, server);
}
//! [is_equal]

int main(void)
{
    VmnlNetAddress server;
    VmnlNetAddress peer;

    if (make_server(&server) == false) {
        return 1;
    }
    print_peer(&server);
    // The same IPv4 address, written mapped into IPv6.
    if (vmnl_net_address_make("::ffff:192.0.2.1", 8080, &peer, NULL) == false) {
        return 1;
    }
    return (is_server(&peer, &server) == true) ? 0 : 1;
}
