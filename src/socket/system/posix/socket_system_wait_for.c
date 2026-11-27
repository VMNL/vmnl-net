#include "socket/system/socket_system_internal.h"

#include <errno.h>

/* NOLINTNEXTLINE(bugprone-easily-swappable-parameters) */
VmnlNetSocketEvents vmnl_net_socket_system_wait_for(const struct VmnlNetSocketSystem *netsock, VmnlNetSocketEvents events, uint64_t timeout, VmnlNetError *error)
{
    struct pollfd pollfd = {.fd = netsock->handle, .events = vmnl_net_socket_system_poll_events(events)};
    uint64_t start = vmnl_net_clock_now();
    int nfds;

    if ((netsock->flags & VMNL_NET_SOCKET_SYSTEM_FLAG_REFUSED) != 0) {
        vmnl_net_error_set(error, VMNL_NET_SUCCESS);
        return events;
    }
    do {
        nfds = poll(&pollfd, 1, vmnl_net_socket_system_poll_timeout(timeout, vmnl_net_clock_elapsed(start)));
    } while ((nfds == -1 && errno == EINTR) || (nfds == 0 && vmnl_net_clock_elapsed(start) < timeout)); /* GCOVR_EXCL_BR_LINE: poll() only fails or ends early on a fault, and after INT_MAX milliseconds. */
    switch (nfds) { /* GCOVR_EXCL_BR_LINE: see the failure below. */
        /* GCOVR_EXCL_START: poll() only fails with a bad pointer, too many descriptors or no memory, which one socket never reaches. */
        case -1:
            vmnl_net_error_set(error, vmnl_net_error_from_last_system());
            return 0;
        /* GCOVR_EXCL_STOP */
        case 0:
            vmnl_net_error_set(error, VMNL_NET_ETIMEDOUT);
            return 0;
        default:
            vmnl_net_error_set(error, VMNL_NET_SUCCESS);
            return vmnl_net_socket_system_ready_events(events, pollfd.revents);
    }
}
