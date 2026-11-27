#include "socket/system/socket_system_internal.h"

/* NOLINTNEXTLINE(bugprone-easily-swappable-parameters) */
VmnlNetSocketEvents vmnl_net_socket_system_wait_for(const struct VmnlNetSocketSystem *netsock, VmnlNetSocketEvents events, uint64_t timeout, VmnlNetError *error)
{
    WSAPOLLFD pollfd = {.fd = netsock->handle, .events = vmnl_net_socket_system_poll_events(events)};
    uint64_t start = vmnl_net_clock_now();
    int nfds;

    do {
        nfds = WSAPoll(&pollfd, 1, vmnl_net_socket_system_poll_timeout(timeout, vmnl_net_clock_elapsed(start)));
    } while (nfds == 0 && vmnl_net_clock_elapsed(start) < timeout);
    switch (nfds) {
        case SOCKET_ERROR:
            vmnl_net_error_set(error, vmnl_net_error_from_last_system());
            return 0;
        case 0:
            vmnl_net_error_set(error, VMNL_NET_ETIMEDOUT);
            return 0;
        default:
            vmnl_net_error_set(error, VMNL_NET_SUCCESS);
            return vmnl_net_socket_system_ready_events(events, pollfd.revents);
    }
}
