#include <vmnl/net/clock.h>

#include "allocator/allocator_internal.h"
#include "context/context_internal.h"
#include "error/error_internal.h"
#include "list/list_internal.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

VmnlNetContext *vmnl_net_context_create(VmnlNetError *error)
{
    VmnlNetContext *context;
#ifdef _WIN32
    WSADATA wsa_data;
    int wsa_error = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (wsa_error != 0) {
        vmnl_net_error_set(error, vmnl_net_error_from_system(wsa_error));
        return NULL;
    }
#endif
    context = vmnl_net_allocator.allocate(sizeof *context);
    if (context == NULL) {
#ifdef _WIN32
        WSACleanup();
#endif
        vmnl_net_error_set(error, VMNL_NET_ENOMEM);
        return NULL;
    }
    *context = (VmnlNetContext) {
        .timers.now = vmnl_net_clock_now(),
    };
    vmnl_net_list_init(&context->timers.active);
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return context;
}
