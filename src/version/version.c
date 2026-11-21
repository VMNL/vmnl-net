#include <vmnl/net/version.h>

#include <stdint.h>

#define VMNL_NET_VERSION_MAJOR_SHIFT 16
#define VMNL_NET_VERSION_MINOR_SHIFT 8

uint32_t vmnl_net_version(void)
{
    return (uint32_t) VMNL_NET_PROJECT_VERSION_MAJOR << VMNL_NET_VERSION_MAJOR_SHIFT | (uint32_t) VMNL_NET_PROJECT_VERSION_MINOR << VMNL_NET_VERSION_MINOR_SHIFT | (uint32_t) VMNL_NET_PROJECT_VERSION_PATCH;
}
