# 0012 Timer structure

- Date: November 22, 2026
- Status: proposed, to decide for 0.5.0

**Context and problem**

From 0.5.0, each reliable message in flight needs a retransmission timer. It starts when the message is sent, stops when the message is acknowledged, and fires when the message is lost.
The deadlines come out of order, as each connection's retransmission timeout follows its own round-trip time and doubles after each timeout, like the RTO of TCP in [RFC 6298](https://www.rfc-editor.org/rfc/rfc6298) and the probe timeout of QUIC in [RFC 9002](https://www.rfc-editor.org/rfc/rfc9002).
The sorted list of [decision 0011](0011-sorted-list-of-timers.md) walks its timers on each start out of order.
A benchmark run on November 22, 2026, on the reference machine, measured a start at 3.3 ns with deadlines in order, and 19 to 21 µs out of order with 32,000 active timers in a new process, up to 26 µs after repeated runs in the same process.
So 1,000 starts out of order take longer than a 60 Hz frame.

The targets below come from assumptions on the load, to confirm with the reliable UDP of 0.5.0.

- A server holds 1,000 connections with 32 reliable messages in flight each, so 32,000 active timers, and starts and stops 1,000 timers per frame.
- The starts, the stops and the expirations of a frame take less than 0.1 ms in all, a hundredth of a 60 Hz frame, whatever the order of the deadlines and with no allocation per start.

**Options considered**

1. The sorted list of [decision 0011](0011-sorted-list-of-timers.md). A stop unlinks the timer at once, but a start out of order walks the timers with a later deadline.
2. A binary heap in an array, where each timer keeps its index. A start and a stop take a logarithmic time, and the array grows with the number of active timers.
3. A timer wheel, which starts and stops a timer in constant time, as Varghese and Lauck describe in [Hashed and Hierarchical Timing Wheels](http://www.cs.columbia.edu/~nahum/w6998/papers/sosp87-timing-wheels.pdf) (SOSP 1987). Their hierarchical wheel moves the timers down its levels as their deadlines come closer. Linux dropped this cascading and gave up accuracy on far deadlines, since most timeouts are canceled before they expire.

**Option chosen**

None yet. [#100](https://github.com/VMNL/vmnl-net/issues/100) measures a wheel and a heap against the sorted list, with the timer benchmarks and a benchmark of a whole frame, and the structure chosen will supersede [decision 0011](0011-sorted-list-of-timers.md).
Whatever the option, the timers keep their API, so the structure can change without breaking it.

The sources are in the [research log](../watch.md).
