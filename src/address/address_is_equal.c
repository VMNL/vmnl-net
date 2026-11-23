#include <vmnl/net/address.h>

#include <string.h>

bool vmnl_net_address_is_equal(const VmnlNetAddress *address, const VmnlNetAddress *other)
{
    return memcmp(address->ip, other->ip, sizeof address->ip) == 0 && address->port == other->port && address->scope == other->scope;
}
