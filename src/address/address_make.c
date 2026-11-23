#include "address/address_internal.h"
#include "error/error_internal.h"

#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

bool vmnl_net_address_make(const char *ip_address, uint16_t port, VmnlNetAddress *address, VmnlNetError *error)
{
    VmnlNetAddress parsed_address = {.port = port};

    // macOS inet_pton() accepts a zone like %1 and drops it.
    if (strchr(ip_address, '%') != NULL) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    switch (inet_pton(AF_INET, ip_address, &parsed_address.ip[VMNL_NET_ADDRESS_IPV4_OFFSET])) {
        case 1:
            memcpy(parsed_address.ip, vmnl_net_address_ipv4_prefix, VMNL_NET_ADDRESS_IPV4_OFFSET);
            break;
        default: {
            if (inet_pton(AF_INET6, ip_address, parsed_address.ip) != 1) {
                vmnl_net_error_set(error, VMNL_NET_EINVAL);
                return false;
            }
        }
    }
    *address = parsed_address;
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
