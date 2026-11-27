# 0020 Waiting on one socket

- Date: November 27, 2026
- Status: accepted

**Context and problem**

A socket never blocks, so a call that would wait fails with `VMNL_NET_EAGAIN`.
Until the poller of [decision 0014](0014-polling.md), an application had to wait with the system's `poll()` or `WSAPoll()` on the native handle, or to retry in a loop. A non-blocking connection also needs a way to learn that it completed.

**Options considered**

1. Leave the wait to the system's functions, through the native handle.
2. A wait on one socket per family, `wait_for()`, with the events to wait for and a timeout.
3. Wait for the poller of the context.

**Option chosen**

Option 2.

- `vmnl_net_socket_datagram_wait_for()` takes `VMNL_NET_SOCKET_READABLE`, `VMNL_NET_SOCKET_WRITABLE` or both, from the shared header `socket.h`, and returns the ones that came. The stream family gets its own.
- The timeout is in nanoseconds, like the clock and the timers, 0 to check without waiting and `VMNL_NET_CLOCK_INF` to wait without limit. It is rounded up to milliseconds, so the wait never ends early.
- A timeout fails with `VMNL_NET_ETIMEDOUT`, so events only come back with `VMNL_NET_SUCCESS`.
- An error waiting for the next call makes the socket ready for every event asked, so the next receive or send reports it.
- A signal never ends the wait. POSIX interrupts `poll()` with `EINTR` whenever a handled signal comes, and the wait goes on for the time left.

**Reason**

The wait hides the differences between `poll()` and `WSAPoll()`, like `POLLPRI`, which makes `WSAPoll()` fail.
Returning `EINTR` was rejected, as only POSIX reports it. `WSAPoll()` documents no such error, and Windows runs a Ctrl-C handler in a new thread, so the wait goes on there anyway. Every handled signal, like `SIGCHLD` or the `SIGPROF` of a profiler, would also force each caller to retry with the time left.
Option 1 would bring the system's headers and their differences into every application, and option 3 would leave 0.3.0 without a way to wait.

**Consequences accepted**

- A wait blocks the calling thread for at most its timeout.
- An application that reacts to Ctrl-C waits with a bounded timeout, and checks its own flag between two waits.
- The poller of the context stays the way to wait on many sockets.

The sources and the tests are in the [research log](../watch.md).
