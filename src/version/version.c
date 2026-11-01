#include <vmnl/net/version.h>

uint32_t vmnl_net_version(void)
{
    return VMNL_NET_PROJECT_VERSION_MAJOR << 16 | VMNL_NET_PROJECT_VERSION_MINOR << 8 | VMNL_NET_PROJECT_VERSION_PATCH;
}
