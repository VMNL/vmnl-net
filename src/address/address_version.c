#include "address/address_internal.h"

VmnlNetAddressVersion vmnl_net_address_version(const VmnlNetAddress *address)
{
    return (memcmp(address->ip, vmnl_net_address_ipv4_prefix, VMNL_NET_ADDRESS_IPV4_OFFSET) == 0) ? VMNL_NET_ADDRESS_IPV4 : VMNL_NET_ADDRESS_IPV6;
}
