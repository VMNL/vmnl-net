# References

The papers and articles the library is built on.

## Dynamic buffers of Boost.Asio

> Buffers, 0.2.0 and 0.3.0

- Source: [Dynamic buffer requirements](https://www.boost.org/doc/libs/latest/doc/html/boost_asio/reference/DynamicBuffer_v2.html) of Boost.Asio and [Networking TS changes to enable better DynamicBuffer composition](https://open-std.org/jtc1/sc22/WG21/docs/papers/2020/p1790r1.html) by Christopher Kohlhoff (WG21, 2020)

The prepare, commit, data and consume model, and the offset reads of its version 2. The paper explains the copy problem behind version 2, which a C handle does not have.

## ByteBuf of Netty

> Buffers, 0.2.0 and 0.3.0

- Source: [ByteBuf](https://netty.io/4.1/api/io/netty/buffer/ByteBuf.html) of Netty

The same model in Java, with reader and writer indices and an explicit compaction, which the library does in prepare().

## basic_multi_buffer of Boost.Beast

> Buffers, 0.2.0 and 0.3.0

- Source: [basic_multi_buffer](https://www.boost.org/doc/libs/latest/libs/beast/doc/html/beast/ref/boost__beast__basic_multi_buffer.html) of Boost.Beast

The same model over a list of blocks, which grows without copying its bytes, like the chunked buffer.

## The Growth Factor Debate

> Buffers, 0.2.0 and 0.3.0

- Source: [The Growth Factor Debate: 1.5x, 2x, or Something Else?](https://pablogs.dev/posts/growth-factor-dynamic-array-1.5-vs-2/) by pablo (2026)

Why the dynamic buffer grows by 1.5 rather than 2.

## Hashed and Hierarchical Timing Wheels

> Timers, 0.2.0 and 0.3.0

- Source: [Hashed and Hierarchical Timing Wheels](http://www.cs.columbia.edu/~nahum/w6998/papers/sosp87-timing-wheels.pdf) by George Varghese and Tony Lauck (SOSP, 1987)

Timer wheels start and stop a timer in constant time. I compare them with the sorted list and the heap.

## Reinventing the timer wheel

> Timers, 0.2.0 and 0.3.0

- Source: [Reinventing the timer wheel](https://lwn.net/Articles/646950/) by Jonathan Corbet (LWN, 2015)

How Linux dropped the cascading of its timer wheel and gave up accuracy, since timeouts are almost always canceled before they expire.

## The high-resolution timer API

> Timers, 0.2.0 and 0.3.0

- Source: [The high-resolution timer API](https://lwn.net/Articles/167897/) by Jonathan Corbet (LWN, 2006)

The Linux timers that must not run late. They sit in a red-black tree, their periodic ones count the periods they missed, and a cancel cannot stop one while its callback runs.

## Beej's Guide to Network Programming

> Sockets, 0.3.0

- Source: [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) by Brian Hall

My starting point for sockets, DNS resolution and polling.

## RFC 768 and RFC 9293

> Sockets, 0.3.0

- Source: [RFC 768](https://www.rfc-editor.org/rfc/rfc768) and [RFC 9293](https://www.rfc-editor.org/rfc/rfc9293)

The UDP and TCP specifications the sockets follow.

## Reading and Writing Packets

> Serialization, 0.4.0

- Source: [Reading and Writing Packets](https://gafferongames.com/post/reading_and_writing_packets/) and [Serialization Strategies](https://gafferongames.com/post/serialization_strategies/) by Glenn Fiedler (Gaffer On Games)

Bit packing, and the bounds checks that reject malicious packets.

## Game networking articles

> Reliable UDP, 0.5.0

- Source: [Game networking articles](https://gafferongames.com/categories/game-networking/) by Glenn Fiedler (Gaffer On Games)

My main source on UDP, for connections, acknowledgments, round-trip time and packet loss.

## Quake 3 Source Code Review

> Reliable UDP, 0.5.0

- Source: [Quake 3 Source Code Review: Network Model](https://fabiensanglard.net/quake3/network.php) by Fabien Sanglard

How a shipped game sends its snapshots over UDP, with delta compression.

## RFC 9002

> Reliable UDP, 0.5.0

- Source: [RFC 9002](https://www.rfc-editor.org/rfc/rfc9002)

Loss detection and congestion control in QUIC, the reference for retransmissions.

## WSAPoll, kqueue and epoll

> Polling and DNS, 0.6.0

- Source: [WSAPoll](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll), [kqueue(2)](https://man.freebsd.org/cgi/man.cgi?query=kqueue) and [epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html)

The exact behavior of each polling API.

## io_uring

> Polling and DNS, 0.6.0

- Source: [Ringing in a new asynchronous I/O API](https://lwn.net/Articles/776703/) by Jonathan Corbet (LWN, 2019) and [On the way to io_uring networking](https://www.youtube.com/watch?v=O0NMUvThafI) by Pavel Begunkov (Kernel Recipes, 2023)

The completion model of Linux, which questions the library's readiness model.

## Packet Fragmentation and Reassembly

> Fragmentation, 0.7.0

- Source: [Packet Fragmentation and Reassembly](https://gafferongames.com/post/packet_fragmentation_and_reassembly/) by Glenn Fiedler (Gaffer On Games)

Fragment headers, a conservative MTU of 1200 bytes, and the checks against malicious fragments.

## RFC 9112

> HTTP, 0.8.0

- Source: [RFC 9112](https://www.rfc-editor.org/rfc/rfc9112)

The HTTP/1.1 specification the HTTP client follows.
