# References

The papers and articles the library is built on, in roadmap order.

**Buffers** (0.2.0, 0.3.0)

- [Dynamic buffer requirements](https://www.boost.org/doc/libs/latest/doc/html/boost_asio/reference/DynamicBuffer_v2.html) of Boost.Asio and [Networking TS changes to enable better DynamicBuffer composition](https://open-std.org/jtc1/sc22/WG21/docs/papers/2020/p1790r1.html) by Christopher Kohlhoff (WG21, 2020): the prepare, commit, data and consume model, the offset reads of its version 2, and the copy problem behind it, which a C handle does not have.
- [ByteBuf](https://netty.io/4.1/api/io/netty/buffer/ByteBuf.html) of Netty: the same model in Java, with reader and writer indices, and an explicit compaction that the library does in prepare().
- [basic_multi_buffer](https://www.boost.org/doc/libs/latest/libs/beast/doc/html/beast/ref/boost__beast__basic_multi_buffer.html) of Boost.Beast: the same model over a list of blocks, which grows without copying its bytes, like the chunked buffer.
- [The Growth Factor Debate: 1.5x, 2x, or Something Else?](https://pablogs.dev/posts/growth-factor-dynamic-array-1.5-vs-2/) by pablo (2026): why the dynamic buffer grows by 1.5 rather than 2.

**Timers** (0.2.0, 0.3.0)

- [Hashed and Hierarchical Timing Wheels](http://www.cs.columbia.edu/~nahum/w6998/papers/sosp87-timing-wheels.pdf) by George Varghese and Tony Lauck (SOSP, 1987): the timer wheels I compare with the sorted list and the heap.
- [Reinventing the timer wheel](https://lwn.net/Articles/646950/) by Jonathan Corbet (LWN, 2015): how Linux removed the cascading from its timer wheel, trading accuracy for timeouts that are almost always canceled before they expire.
- [The high-resolution timer API](https://lwn.net/Articles/167897/) by Jonathan Corbet (LWN, 2006): the timers of Linux that must not run late, kept in a red-black tree, whose periodic ones count the periods they missed, and which a cancel cannot stop while their callback runs.

**Sockets** (0.3.0)

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) by Brian Hall: my starting point for sockets, DNS resolution and polling.
- [RFC 768](https://www.rfc-editor.org/rfc/rfc768) and [RFC 9293](https://www.rfc-editor.org/rfc/rfc9293): the UDP and TCP specifications the sockets follow.

**Serialization** (0.4.0)

- [Reading and Writing Packets](https://gafferongames.com/post/reading_and_writing_packets/) and [Serialization Strategies](https://gafferongames.com/post/serialization_strategies/) by Glenn Fiedler (Gaffer On Games): bit packing, and the bounds checks that reject malicious packets.

**Reliable UDP** (0.5.0)

- [Game networking articles](https://gafferongames.com/categories/game-networking/) by Glenn Fiedler (Gaffer On Games): my main source on UDP, for connections, acknowledgments, round-trip time and packet loss.
- [Quake 3 Source Code Review: Network Model](https://fabiensanglard.net/quake3/network.php) by Fabien Sanglard: how a shipped game sends its snapshots over UDP, with delta compression.
- [RFC 9002](https://www.rfc-editor.org/rfc/rfc9002): loss detection and congestion control in QUIC, the reference for retransmissions.

**Polling and DNS** (0.6.0)

- [WSAPoll](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll), [kqueue(2)](https://man.freebsd.org/cgi/man.cgi?query=kqueue) and [epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html): the exact behavior of each polling API.
- [Ringing in a new asynchronous I/O API](https://lwn.net/Articles/776703/) by Jonathan Corbet (LWN, 2019) and [On the way to io_uring networking](https://www.youtube.com/watch?v=O0NMUvThafI) by Pavel Begunkov (Kernel Recipes, 2023): io_uring, the completion model of Linux, which questions the readiness model of the library.

**Fragmentation** (0.7.0)

- [Packet Fragmentation and Reassembly](https://gafferongames.com/post/packet_fragmentation_and_reassembly/) by Glenn Fiedler (Gaffer On Games): fragment headers, a conservative MTU of 1200 bytes, and the checks against malicious fragments.

**HTTP** (0.8.0)

- [RFC 9112](https://www.rfc-editor.org/rfc/rfc9112): the HTTP/1.1 specification the HTTP client follows.
