# 0007 No background thread

- Date: October 3, 2026
- Status: accepted

**Context and problem**

A game calls the library from its own loop, once per frame, and decides when the network work happens.
A network library can either run threads of its own or run inside the application's calls.

**Options considered**

1. Threads inside the library, which send, receive and fire timers on their own.
2. No thread of its own. Every function runs inside an application call, on the thread that owns the context.

**Option chosen**

Option 2.

**Reason**

The library takes no lock and has no data race, and a bug reproduces under a debugger.

**Consequences accepted**

- The application runs the loop and gives its context the time through `vmnl_net_context_advance()`.
- A context belongs to one thread at a time, so a program that spreads its network over several threads creates one context per thread.
- The network only progresses inside the application's calls. A game loop that blocks or computes for long has to run the network in a thread of its own.
- A blocking call stalls the application's loop, so the blocking resolution of [0005](0005-dns-resolution.md) would need a documented exception.
