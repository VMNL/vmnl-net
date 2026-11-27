#define _GNU_SOURCE

#include "../socket_datagram_tests.h"

#include <signal.h>
#include <sys/time.h>

static void ignore_signal(int signal_number)
{
    (void) signal_number;
}

DATAGRAM_PAIR_FIXTURE_I(vmnl_net_socket_datagram_wait_for, types)

UTEST_I(vmnl_net_socket_datagram_wait_for, keeps_waiting_after_signal, NTYPES)
{
    VmnlNetError error = UINT32_MAX;
    struct sigaction previous;
    VmnlNetSocketEvents events;
    uint64_t elapsed;
    uint64_t start;

    ASSERT_EQ(sigaction(SIGALRM, &(struct sigaction) {.sa_handler = ignore_signal}, &previous), 0);
    ASSERT_EQ(setitimer(ITIMER_REAL, &(struct itimerval) {.it_value = {.tv_usec = 40000}}, NULL), 0);
    start = vmnl_net_clock_now();
    events = vmnl_net_socket_datagram_wait_for(utest_fixture->receiver, VMNL_NET_SOCKET_READABLE, 50 * MILLISECOND, &error);
    elapsed = vmnl_net_clock_elapsed(start);
    setitimer(ITIMER_REAL, &(struct itimerval) {0}, NULL);
    sigaction(SIGALRM, &previous, NULL);
    EXPECT_EQ(events, (VmnlNetSocketEvents) 0);
    EXPECT_EQ(error, VMNL_NET_ETIMEDOUT);
    EXPECT_GE(elapsed, 50 * MILLISECOND);
    EXPECT_LT(elapsed, 75 * MILLISECOND);
}
