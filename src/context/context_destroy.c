#include "allocator/allocator_internal.h"
#include "context/context_internal.h"
#include "timer/timer_internal.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

void vmnl_net_context_destroy(VmnlNetContext *context)
{
    VmnlNetTimer *timer;

    if (context == NULL) {
        return;
    }
    VMNL_NET_LIST_FOREACH(timer, &context->timers.active, link)
    {
        vmnl_net_list_remove(&timer->link);
    }
    vmnl_net_allocator.deallocate(context);
#ifdef _WIN32
    WSACleanup();
#endif
}
