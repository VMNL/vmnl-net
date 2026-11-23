#include "address/address_internal.h"
#include "error/error_internal.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

static bool vmnl_net_address_make_has_leading_zero(const char *ip_address)
{
    const char *ipv6 = strrchr(ip_address, ':');
    const char *ipv4 = (ipv6 == NULL) ? ip_address : ipv6 + 1;

    if (strchr(ipv4, '.') == NULL) {
        return false;
    }
    for (size_t it = 0; ipv4[it] != '\0'; it++) {
        if ((it == 0 || ipv4[it - 1] == '.') && ipv4[it] == '0' && isdigit((unsigned char) ipv4[it + 1]) != 0) {
            return true;
        }
    }
    return false;
}

bool vmnl_net_address_make(const char *ip_address, uint16_t port, VmnlNetAddress *address, VmnlNetError *error)
{
    VmnlNetAddress parsed_address = {.port = port};

    if (strchr(ip_address, '%') != NULL || vmnl_net_address_make_has_leading_zero(ip_address) == true) {
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
