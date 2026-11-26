#include <vmnl/net/address.h>

#include <stdbool.h>
#include <stdio.h>

//! [make]
static bool fill_server_address(VmnlNetAddress *server_address)
{
    VmnlNetError error;

    // A host name like localhost is refused, only an IP is parsed.
    if (vmnl_net_address_make("192.0.2.1", 8080, server_address, &error) == false) {
        printf("%s\n", vmnl_net_error_as_string(error));
        return false;
    }
    return vmnl_net_address_version(server_address) == VMNL_NET_ADDRESS_IPV4; // true
}
//! [make]

//! [as_string]
static void log_peer_address(const VmnlNetAddress *peer)
{
    char ip_address[VMNL_NET_ADDRESS_AS_STRING_SIZE];

    vmnl_net_address_as_string(peer, ip_address, sizeof ip_address, NULL);
    if (vmnl_net_address_version(peer) == VMNL_NET_ADDRESS_IPV4) {
        printf("%s:%u\n", ip_address, (unsigned) peer->port); // 192.0.2.1:8080
    } else {
        printf("[%s]:%u\n", ip_address, (unsigned) peer->port); // [2001:db8::1]:8080
    }
}
//! [as_string]

//! [is_equal]
static bool is_server_address(const VmnlNetAddress *peer, const VmnlNetAddress *server)
{
    // Never memcmp(), as the struct has padding bytes.
    return vmnl_net_address_is_equal(peer, server);
}
//! [is_equal]

int main(void)
{
    VmnlNetAddress server_address;
    VmnlNetAddress peer_address;

    if (fill_server_address(&server_address) == false) {
        return 1;
    }
    log_peer_address(&server_address);
    // The same IPv4 address, written mapped into IPv6.
    if (vmnl_net_address_make("::ffff:192.0.2.1", 8080, &peer_address, NULL) == false) {
        return 1;
    }
    return is_server_address(&peer_address, &server_address) == false;
}
