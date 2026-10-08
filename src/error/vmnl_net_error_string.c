#include <vmnl/net/error.h>

/**
 * \brief Error codes mapping table with their human-readable messages.
 */
static const char *const vmnl_net_error_strings[] = {
    [VMNL_NET_SUCCESS] = "Success",
    [VMNL_NET_EINVAL]  = "Invalid argument",
    [VMNL_NET_ENOMEM]  = "Cannot allocate memory",
    [VMNL_NET_FSYSTEM] = "System error",
};

const char *vmnl_net_error_string(VmnlNetError error)
{
    if (error >= sizeof vmnl_net_error_strings / sizeof vmnl_net_error_strings[0]) {
        return "Unknown error";
    }
    return vmnl_net_error_strings[error];
}
