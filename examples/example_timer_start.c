#include <vmnl/net/clock.h>
#include <vmnl/net/timer.h>

#include <stdint.h>
#include <stdio.h>

//! [callback]
static void send_heartbeat(VmnlNetTimer *timer, uint64_t lateness, void *data)
{
    unsigned *nheartbeats = data;

    (*nheartbeats)++;
    printf("Heartbeat %u, %llu ns late\n", *nheartbeats, (unsigned long long) lateness);
    // A callback can stop its own timer
    if (*nheartbeats == 3) {
        vmnl_net_timer_stop(timer);
    }
}
//! [callback]

int main(void)
{
    VmnlNetError error = VMNL_NET_SUCCESS;
    VmnlNetContext *context = vmnl_net_context_create(&error);

    if (context == NULL) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        return 1;
    }

    //! [start]
    unsigned nheartbeats = 0;
    VmnlNetTimer *heartbeat = vmnl_net_timer_create(send_heartbeat, &nheartbeats, &error);

    if (heartbeat == NULL) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error));
        vmnl_net_context_destroy(context);
        return 1;
    }
    // First deadline in 50 ms, then every 100 ms
    vmnl_net_timer_set_interval(heartbeat, VMNL_NET_NANOSECONDS_PER_SECOND / 10);
    vmnl_net_timer_start(context, heartbeat, VMNL_NET_NANOSECONDS_PER_SECOND / 20);
    //! [start]

    //! [advance]
    // The loop of the application, here until the callback stops the heartbeat
    while (vmnl_net_timer_is_active(heartbeat) == true) {
        uint64_t now = vmnl_net_clock_now();

        // The frame of the game, with its inputs, network and rendering,
        // or a server's wait of vmnl_net_context_remaining(context, now) nanoseconds
        vmnl_net_context_advance(context, now);
    }
    //! [advance]

    vmnl_net_timer_destroy(heartbeat);
    vmnl_net_context_destroy(context);
    return 0;
}
