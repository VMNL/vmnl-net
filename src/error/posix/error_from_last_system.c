#include "error/error_internal.h"

#include <errno.h>

VmnlNetError vmnl_net_error_from_last_system(void)
{
    return vmnl_net_error_from_system(errno);
}
