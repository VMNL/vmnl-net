# 0011 Sorted list of timers

- Date: October 12, 2026
- Status: accepted, to be superseded by [0012](0012-timer-structure.md) once a wheel or a heap is measured

**Context and problem**

A context keeps its active timers and fires the expired ones when the application gives it the time.
The structure decides the cost of starting, stopping and firing a timer.

**Options considered**

1. An intrusive list sorted by deadline, ported from the list of the Linux kernel.
2. A binary heap.
3. A timer wheel.

**Option chosen**

Option 1. A new timer is inserted from the back of the list, and the expired ones are taken from its front.

**Reason**

It is the simplest structure, and it was kept until benchmarks show the need for another one.
Stopping a timer unlinks it at once, and equal timeouts insert at once from the back.

**Consequences accepted**

- A timer with an earlier deadline than the others walks the list on its start.
- On November 22, 2026, on the reference machine, a start took 3.3 ns with deadlines in order, and 19 to 21 µs out of order with 32,000 active timers in a new process, up to 26 µs after repeated runs in the same process.
- A wheel and a heap come next, measured with the same benchmarks, and the one chosen will supersede this decision.
