#include "vmnl/net.h"

uint32_t vmnl_net_version(void)
{
    return VMNL_NET_VERSION_MAJOR << 16 | VMNL_NET_VERSION_MINOR << 8 | VMNL_NET_VERSION_PATCH;
}
