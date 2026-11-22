#include <vmnl/net/version.h>

#include <stdint.h>
#include <stdio.h>

int main(void)
{
    //! [version]
    uint32_t version = vmnl_net_version();

    // Same comparison in the following if statement, version is packed as MAJOR|MINOR|PATCH
    if ((VMNL_NET_VERSION_MAJOR(version) <= 0 && VMNL_NET_VERSION_MINOR(version) < 3) || version < 0x000300) {
        fprintf(stderr, "vmnl_net 0.3 or later is required, found %s\n", vmnl_net_version_string());
        return 1;
    }
    //! [version]
    return 0;
}
