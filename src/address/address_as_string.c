#include "address/address_internal.h"
#include "error/error_internal.h"

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

bool vmnl_net_address_as_string(const VmnlNetAddress *address, char *bytes, size_t nbytes, VmnlNetError *error)
{
    if (nbytes < VMNL_NET_ADDRESS_AS_STRING_SIZE) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    if (vmnl_net_address_version(address) == VMNL_NET_ADDRESS_IPV4) {
        inet_ntop(AF_INET, &address->ip[VMNL_NET_ADDRESS_IPV4_OFFSET], bytes, VMNL_NET_ADDRESS_AS_STRING_SIZE);
    } else {
        inet_ntop(AF_INET6, address->ip, bytes, VMNL_NET_ADDRESS_AS_STRING_SIZE);
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
