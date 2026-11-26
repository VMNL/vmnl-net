# 0014 Polling

- Date: November 25, 2026
- Status: proposed, to decide for 0.6.0

**Context and problem**

From 0.6.0, an application waits for many sockets in one call, through epoll on Linux, kqueue on macOS and WSAPoll on Windows.
A wait on one socket at a time cannot do it. It blocks on one socket while others have data, or costs a system call per socket and per frame.
The library has no thread of its own, as [decision 0007](0007-no-background-thread.md) explains, so the wait runs inside an application call. The context already gives its timeout through `vmnl_net_context_remaining()`.
Every family can be polled. A listener is ready when a connection waits, a stream when data arrives or the peer closes, and a datagram socket when a datagram arrives. A non-blocking connect ends when its stream becomes writable.
The Handle base of [decision 0001](0001-socket-model.md) exists for this wait, its only caller that handles objects of every family without knowing them.

**Options considered**

1. A poller object next to the context, like mio's `Poll`. The application registers each socket it watches, with the events it waits for.
2. A reactor inside the context, which waits through `vmnl_net_context_poll()`, like libuv's loop runs its timers and its I/O.
   - Each socket joins the reactor when it is created in its context.
   - The reactor stays internal. It wraps epoll, kqueue or WSAPoll, picked at compile time like the sockets' `posix` and `win32` code.
   - It calls the operations of the Handle base, which each kind fills in. A pair, without a system socket, signals itself to the context, which shortens its wait.
   - The library owns the send buffers, so it watches the writable sockets only while it has bytes to send.
   - `vmnl_net_context_poll()` waits until the next timer deadline at most, runs the I/O into the library's buffers and fills a list of events. Each event has a type, like accepted, received or closed, and its fields depend on that type.
   - The application reads the list, or sets one callback per type of event on the context and calls `vmnl_net_context_dispatch()`. The poll itself never calls them, so no event is handled twice.
   - Each socket carries a user data pointer, so a callback finds the application's object.
   - The application cannot choose which sockets the context watches, and the same events come in two ways, to document and test.

**Option chosen**

None yet, to choose for 0.6.0. Option 2 is the one explored so far, and these questions stay open.

- Whether the wait is edge-triggered or level-triggered.
- Whether `vmnl_net_context_advance()` dispatches the events instead of a new function, as it already calls the timers' callbacks.
- How WSAPoll, which takes every socket at each call, follows the sockets that join and leave.
- How a notifier wakes the wait from another thread.

Whatever the option, the Handle base comes with the wait, so its operations follow the option chosen.

The sources are in the [research log](../watch.md).
