#include <vmnl/net.h>

#include <stdio.h>

//! [callbacks]
static void on_tick(VmnlNetTimer *timer, void *user_data)
{
    (void) timer;
    (void) user_data;
    printf("Tick\n");
}

static void on_timeout(VmnlNetTimer *timer, void *user_data)
{
    (void) timer;
    *(bool *) user_data = false; // Ends the loop
}
//! [callbacks]

static void wait_for_network(uint64_t timeout)
{
    uint64_t start = vmnl_net_clock_now();

    // No sockets yet, so it waits out the timeout, where a poll() would return early on a packet
    while (vmnl_net_clock_now() - start < timeout) {
    }
}

int main(void)
{
    VmnlNetContext *context = vmnl_net_context_create(NULL);
    bool running = true;
    //! [start]
    VmnlNetTimer *tick = vmnl_net_timer_create(context, on_tick, NULL, NULL);
    VmnlNetTimer *timeout = vmnl_net_timer_create(context, on_timeout, &running, NULL);
    uint64_t now = vmnl_net_clock_now();

    vmnl_net_timer_start(context, tick, now, 0, 10 * VMNL_NET_NANOSECONDS_PER_MILLISECOND); // At the next update, then every 10 ms
    vmnl_net_timer_start(context, timeout, now, 50 * VMNL_NET_NANOSECONDS_PER_MILLISECOND, 0); // Once, in 50 ms
    //! [start]
    //! [loop]
    while (running) {
        wait_for_network(vmnl_net_timer_remaining(context, vmnl_net_clock_now())); // Until a packet arrives or the next timer is due
        vmnl_net_timer_update(context, vmnl_net_clock_now()); // Calls the callbacks of the timers due by now, on_tick and on_timeout here
    }
    //! [loop]
    vmnl_net_timer_destroy(context, timeout);
    vmnl_net_timer_destroy(context, tick);
    vmnl_net_context_destroy(context);
    return 0;
}
