#include <vmnl/net/address.h>

bool vmnl_net_address_is_ipv6(const VmnlNetAddress *address)
{
    return vmnl_net_address_is_ipv4(address) == false;
}
