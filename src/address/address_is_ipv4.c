#include "address/address_internal.h"

bool vmnl_net_address_is_ipv4(const VmnlNetAddress *address)
{
    return memcmp(address->ip, vmnl_net_address_ipv4_prefix, VMNL_NET_ADDRESS_IPV4_OFFSET) == 0;
}
