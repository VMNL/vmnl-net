# Watch

This file keeps track of every watch this library's decisions are built on, see [references](references.md) for papers and articles.

The watch follows the RFCs, LWN.net for the networking and the timers of Linux, Gaffer On Games for the netcode of games, and the docs of Microsoft, man7.org and Apple for the system calls.
It also follows the releases of the candidate dependencies, like c-ares, OpenSSL and BearSSL, and of libraries with the same goals, like libuv, ENet and SDL_net.

## The intrusive list of Linux

> October 12, 2026

- Status: Adopted in [6aa55e1](https://github.com/VMNL/vmnl-net/commit/6aa55e1)
- Source: The intrusive list of Linux, [include/linux/list.h](https://github.com/torvalds/linux/blob/master/include/linux/list.h)

#### Subject

A generic doubly linked list whose links live inside the structures they chain.

- What it changes: One list serves every module that chains structures, without allocating per link
- What I did: Ported it as the library's intrusive list, first for a context's active timers sorted by deadline, now also for the chunked buffer's chunks

## Process alternation and random interleaving

> November 22, 2026

- Status: Tested
- Source: [random interleaving](https://github.com/google/benchmark/blob/v1.9.5/docs/random_interleaving.md) and [paused timing](https://github.com/google/benchmark/blob/v1.9.5/docs/user_guide.md) in the docs of Google Benchmark 1.9.5, and [Rigorous Benchmarking in Reasonable Time](https://doi.org/10.1145/2464157.2464160) by Tomas Kalibera and Richard Jones (ISMM 2013)

#### Subject

Google Benchmark can interleave the repetitions of different benchmarks at random, and warns that pausing the timer has a high overhead. Kalibera and Jones repeat the runs at every level where the results vary, the executions included.

- What it changes: On 25 benchmarks run against themselves, 9 repetitions in one process gave 12 false changes, random interleaving 6, 3 processes of 3 repetitions 5, and 9 processes of 1 repetition none. On all 179 benchmarks, 9 processes gave 4 false changes instead of 122. Timing by hand instead of pausing the timer made the run 43 % longer without lowering the noise
- What I did: tools/compare_benchmarks.sh runs each build 9 times in turn, each run in a new process, as [decision 0004](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0004-benchmark-comparison-on-a-reference-machine.md) explains

## Mutation testing with mutmut and Mull

> November 24, 2026

- Status: Adopted in [63392a7](https://github.com/VMNL/vmnl-net/commit/63392a7)
- Source: A [reel by Stanislav Zmiev](https://www.instagram.com/reel/DgGDjy5A2hy/) on mutation testing with mutmut, and the [docs of Mull 0.39.1](https://mull.readthedocs.io/en/latest/)

#### Subject

Mutation testing changes the code one small edit at a time and runs the tests on each change. A change that no test catches shows code that runs without being checked. mutmut only handles Python, and Mull handles C through a plugin of Clang, on Linux and macOS.

- What it changes: The tests caught 445 of 493 mutants on Linux, 446 of 488 on macOS, and 457 of 498 once both reports are merged. The 7 mutants in the address module's leading zero check survive on Linux, where inet_pton already refuses leading zeros, and are caught on macOS. No test checks the error code of a chunked buffer created with a capacity of 0
- What I did: Ran Mull 0.39.1 with LLVM 19 on Ubuntu 24.04 in a container and on macOS 26.5.1, merged both reports with jq, added the missing tests, simplified the code that served nothing, left out the boundary mutators, and added a CI job on both systems, as [decision 0013](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0013-mutation-testing.md) explains

## SIO_UDP_CONNRESET in OpenJDK, Go, libuv and ENet

> November 26, 2026

- Status: Tested
- Source: [Winsock IOCTLs](https://learn.microsoft.com/en-us/windows/win32/winsock/winsock-ioctls) in the docs of Microsoft, the Windows code of [OpenJDK](https://github.com/openjdk/jdk/blob/master/src/java.base/windows/native/libnio/ch/Net.c), [libuv](https://github.com/libuv/libuv/blob/v1.x/src/win/udp.c) and [ENet](https://github.com/lsalzman/enet/blob/master/win32.c), and the [Windows code of Go](https://github.com/golang/go/blob/master/src/net/fd_windows.go) with its issues [5834](https://github.com/golang/go/issues/5834) and [68614](https://github.com/golang/go/issues/68614)

#### Subject

Windows reports an ICMP port unreachable on every UDP socket, connected or not, as WSAECONNRESET at the next receive. It reports an expired time to live as WSAENETRESET. OpenJDK clears SIO_UDP_CONNRESET when it creates a datagram socket, and sets it again after a connect. Go clears SIO_UDP_CONNRESET and SIO_UDP_NETRESET on every UDP socket and never sets them again. libuv and ENet set neither, and ignore WSAECONNRESET at each receive.

- What it changes: A client that disappears makes the next receive of a Windows server fail. In a test, an unconnected socket gave EAGAIN on Linux and macOS and WSAECONNRESET on Windows, and a connected socket gave ECONNREFUSED on Linux and macOS and WSAECONNRESET on Windows
- What I did: Followed OpenJDK, and also cleared SIO_UDP_NETRESET like Go, as [decision 0019](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0019-icmp-errors-on-windows.md) explains. A connected socket now reports a port unreachable as VMNL_NET_ECONNREFUSED on every system

## Socket inheritance after accept on Windows

> November 26, 2026

- Status: Tested
- Source: [accept](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept), [WSAAccept](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaaccept) and [WSASocketW](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsasocketw) in the docs of Microsoft, the post [accept() and socket inheritance](https://purplesyringa.moe/blog/accept-and-socket-inheritance/), the Windows code of [CPython](https://github.com/python/cpython/blob/main/Modules/socketmodule.c) and [Rust](https://github.com/rust-lang/rust/blob/master/library/std/src/sys/net/connection/socket/windows.rs), and the review of [libplctag #678](https://github.com/libplctag/libplctag/pull/678)

#### Subject

The docs of Microsoft say that a socket from accept() has the same properties as its listener, without naming the inheritance. The post found that an accepted socket follows the flag its listener got at creation, not one set later with SetHandleInformation(), and that Wine always returns inheritable sockets. Rust relies on the listener, while CPython and libplctag still clear the flag after accept().

- What it changes: An accepted socket is not inheritable when its listener was created with WSA_FLAG_NO_HANDLE_INHERIT. In a test on Windows 11, the IPv4 and IPv6 sockets accepted by such a listener kept its flag
- What I did: Kept SetHandleInformation() after accept(), as [decision 0017](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0017-system-socket.md) explains, since Microsoft does not document the behavior and Wine differs

## Apple App Review Guidelines, rule 2.5.5

> November 19, 2026

- Status: Planned
- Source: Apple [App Review Guidelines](https://developer.apple.com/app-store/review/guidelines/), rule 2.5.5, [IPv6-only networks](https://developer.apple.com/support/ipv6/) and [Supporting IPv6 DNS64/NAT64 Networks](https://developer.apple.com/library/archive/documentation/NetworkingInternetWeb/Conceptual/NetworkingOverview/UnderstandingandPreparingfortheIPv6Transition/UnderstandingandPreparingfortheIPv6Transition.html)

#### Subject

The App Store requires apps to work on IPv6-only networks, where getaddrinfo turns an IPv4 literal into a synthesized IPv6 address.

- What it changes: inet_pton parses an IPv4 literal as an IPv4 address, which reaches nothing on such a network
- What I did: A remote peer's address goes through the resolution, even when it is a numeric IP

## AF_UNIX sockets on Windows

> November 19, 2026

- Status: Planned
- Source: [AF_UNIX comes to Windows](https://devblogs.microsoft.com/commandline/af_unix-comes-to-windows/), Microsoft, 2017, and a test of [WSAPoll](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll) on AF_UNIX sockets, on Windows 10.0.26300

#### Subject

AF_UNIX works over Winsock since Windows 10 build 17063, for SOCK_STREAM only. In a test with WSAPoll, these sockets behave like TCP, except that a closed peer gives POLLHUP alone and a connect without a listener fails at once.

- What it changes: The public API can only offer local streams, and the poll has to read POLLHUP as readable and accept a connect that fails at once
- What I did: Local stream and listener sockets join the public API in [decision 0001](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0001-socket-model.md). Local datagrams, SOCK_SEQPACKET and pipes stay out

## Socket APIs of libuv, ENet, SDL_net, Rust, Go, Asio, .NET and Java

> November 19, 2026

- Status: Planned
- Source: Documentation of [libuv](https://docs.libuv.org/en/v1.x/tcp.html), [ENet](https://github.com/lsalzman/enet), [SDL_net](https://github.com/libsdl-org/SDL_net), Rust [std::net](https://doc.rust-lang.org/std/net/) and [socket2](https://docs.rs/socket2), [Go](https://pkg.go.dev/net), [Asio](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/reference/local__stream_protocol.html), [.NET](https://learn.microsoft.com/en-us/dotnet/api/system.net.endpoint) and [Java](https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/net/SocketAddress.html)

#### Subject

Some libraries have one generic socket with a base address, like ENet or .NET. Others type their sockets with an address per family, like Rust or Asio, or around a shared base, like libuv or Go.

- What it changes: A flat list of typed sockets repeats its functions and cannot grow to new families
- What I did: [Decision 0001](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0001-socket-model.md) chose typed families (Stream, Listener, Message, Datagram and Notifier) under a Handle base, dispatched through operation tables like the buffers, with options set through setters

## RFC 8085

> November 18, 2026

- Status: Planned
- Source: [RFC 8085](https://www.rfc-editor.org/rfc/rfc8085)

#### Subject

Without path MTU discovery, a UDP datagram should fit an IP packet of 576 bytes in IPv4 and 1280 in IPv6, since some NATs and firewalls drop fragments.

- What it changes: Without discovery, a datagram carries at most 1232 bytes over IPv6, after the 40-byte IPv6 header and the 8-byte UDP header
- What I did: RUDP splits its messages below the MTU, and the plain UDP docs give the safe size

## RFC 896

> November 18, 2026

- Status: Planned
- Source: [RFC 896](https://www.rfc-editor.org/rfc/rfc896)

#### Subject

Nagle's algorithm holds new small segments while sent data is unacknowledged.

- What it changes: A small TCP write, like a game input, can wait for the previous one's acknowledgment
- What I did: A TCP_NODELAY setter for TCP sockets

## Porting socket applications to Winsock

> November 8, 2026

- Status: Planned
- Source: [Porting socket applications to Winsock](https://learn.microsoft.com/en-us/windows/win32/winsock/porting-socket-applications-to-winsock)

#### Subject

Winsock differs from BSD sockets in details like the socket type, an unsigned `SOCKET`, and `closesocket()` instead of `close()`.

- What it changes: The socket wrapper has to hide these differences
- What I did: Drafted an internal wrapper that hides them, to redo with the 0.3.0 socket model

## The high-resolution timer API

> November 1, 2026

- Status: Adopted in [4446f5c](https://github.com/VMNL/vmnl-net/commit/4446f5c)
- Source: [The high-resolution timer API](https://lwn.net/Articles/167897/) by Jonathan Corbet (LWN, 2006)

#### Subject

In the high-resolution timers of Linux, a late periodic timer counts the periods it missed, and a cancel cannot stop a timer while its callback runs.

- What it changes: The library's timers have to settle what a late periodic timer reports, and whether a callback can stop its own timer
- What I did: The timer rework passes the callback how late it runs instead of a count of missed periods, keeps periodic timers in phase, and lets a callback stop, start or destroy any timer, its own included

## Reinventing the timer wheel

> October 17, 2026

- Status: Planned
- Source: [Reinventing the timer wheel](https://lwn.net/Articles/646950/) by Jonathan Corbet (LWN, 2015)

#### Subject

Linux dropped the cascading of its timer wheel and gave up accuracy, since timeouts are almost always canceled before they expire.

- What it changes: A network library's timeouts, like retransmissions, are mostly canceled too, so the model is worth a look
- What I did: The timer benchmarks rule out the sorted list for deadlines out of order, so a wheel is to prototype against a heap, in [decision 0012](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0012-timer-structure.md)

## Hashed and Hierarchical Timing Wheels

> October 17, 2026

- Status: Planned
- Source: [Hashed and Hierarchical Timing Wheels](http://www.cs.columbia.edu/~nahum/w6998/papers/sosp87-timing-wheels.pdf) by George Varghese and Tony Lauck (SOSP, 1987)

#### Subject

A timer wheel starts and stops a timer in constant time, where a sorted list has to find the place of each new deadline.

- What it changes: The library's sorted list searches from its back, so it can walk all its timers on each start unless their timeouts are equal
- What I did: The benchmarks confirmed it on the reference machine. A start costs 3.3 ns with deadlines in order, but 19 to 21 µs out of order with 32,000 active timers in a new process, up to 26 µs after repeated runs in the same process, so the 1,000 starts of a frame take longer than a 60 Hz frame. A wheel and a heap come next, measured with the same benchmarks in [decision 0012](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0012-timer-structure.md)

## Polling APIs of libuv, ENet and mio

> November 25, 2026

- Status: Deferred to 0.6.0
- Source: Documentation of [libuv](https://docs.libuv.org/en/v1.x/design.html) and its [streams](https://docs.libuv.org/en/v1.x/stream.html), the [header of ENet](https://github.com/lsalzman/enet/blob/master/include/enet/enet.h) and the [Poll of mio](https://docs.rs/mio/latest/mio/struct.Poll.html)

#### Subject

All three keep the watched sockets in one object. libuv runs its timers and its I/O poll in the same loop iteration, over epoll, kqueue or IOCP, and reports through callbacks like the read callback of `uv_read_start()`. ENet's `enet_host_service()` returns one event at a time, a connect, a disconnect or a receive. mio registers each socket with a token and an interest, then `poll()` fills a list of events, and its page says nothing of timers.

- What it changes: A wait on many sockets needs an object that keeps them, and a library with timers runs them in the same loop
- What I did: [Decision 0014](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0014-polling.md) proposes a reactor inside the context, with events for the application and callbacks per type of event

## recvmmsg, sendmmsg, WSARecvMsg and Registered I/O

> November 26, 2026

- Status: Deferred to 0.6.0
- Source: [recvmmsg(2)](https://man7.org/linux/man-pages/man2/recvmmsg.2.html) and [sendmmsg(2)](https://man7.org/linux/man-pages/man2/sendmmsg.2.html) on man7.org, [WSARecvMsg](https://learn.microsoft.com/en-us/windows/win32/api/mswsock/nc-mswsock-lpfn_wsarecvmsg) and [Registered I/O](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-r2-and-2012/hh997032(v=ws.11)) in the docs of Microsoft

#### Subject

recvmmsg and sendmmsg move several datagrams in one system call, since Linux 2.6.33 and 3.0, and the SDK of macOS declares neither. WSARecvMsg receives one datagram with its control data, like IP_PKTINFO, through a pointer that WSAIoctl gives. Registered I/O queues sends and receives on registered buffers and reports them through completion queues, since Windows Server 2012.

- What it changes: Batching saves system calls on a busy UDP socket, but only on Linux, and one call then receives several datagrams, each with its own address and size. Registered I/O reports finished operations instead of ready sockets, unlike the poll of [decision 0014](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0014-polling.md). WSARecvMsg only adds control data, which no feature needs yet
- What I did: Kept sendmsg and recvmsg on POSIX, and WSASendTo and WSARecvFrom on Windows, one datagram per call. Batching waits for the UDP benchmarks and for the poll of 0.6.0

## DNS RFCs and the DNS configuration of macOS

> November 22, 2026

- Status: Deferred to 0.6.0
- Source: [RFC 1035](https://www.rfc-editor.org/rfc/rfc1035), [RFC 3596](https://www.rfc-editor.org/rfc/rfc3596), [RFC 7766](https://www.rfc-editor.org/rfc/rfc7766), [RFC 7050](https://www.rfc-editor.org/rfc/rfc7050), [RFC 6052](https://www.rfc-editor.org/rfc/rfc6052) and the [notice that configd writes in /etc/resolv.conf](https://github.com/apple-oss-distributions/configd/blob/585b7f2fca293f4642d21d15c5daf187f63c4796/Plugins/IPMonitor/ip_plugin.c#L5893-L5897)

#### Subject

The DNS protocol itself is small, queries over UDP with a TCP fallback. The system resolver does much more. macOS reads its DNS settings from configd and ignores `/etc/resolv.conf`, and on IPv6-only networks it builds IPv6 addresses for IPv4 literals from the NAT64 prefix.

- What it changes: Our own resolver would redo the system's work rather than the protocol. It would read each system's DNS settings and build the NAT64 addresses, and lose mDNS, VPN split DNS and the system's encrypted DNS
- What I did: Option 2 of [decision 0005](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0005-dns-resolution.md)

## c-ares

> November 19, 2026

- Status: Deferred to 0.6.0
- Source: c-ares [options](https://c-ares.org/docs/ares_init_options.html), [ares_process](https://c-ares.org/docs/ares_process.html), [ares_timeout](https://c-ares.org/docs/ares_timeout.html), [ares_reinit](https://c-ares.org/docs/ares_reinit.html), its configuration readers for [Linux](https://github.com/c-ares/c-ares/blob/main/src/lib/ares_sysconfig_files.c), [macOS](https://github.com/c-ares/c-ares/blob/main/src/lib/ares_sysconfig_mac.c) and [Windows](https://github.com/c-ares/c-ares/blob/main/src/lib/ares_sysconfig_win.c), and its [license](https://github.com/c-ares/c-ares/blob/main/LICENSE.md)

#### Subject

c-ares is a DNS stub resolver under MIT with no event loop of its own. The application adds its sockets to its own poll, and calls c-ares when they are ready or when its next timeout expires. It reads the system settings on Linux, macOS and Windows, and `ares_reinit()` reloads them, from a background thread when c-ares is built thread-safe.

- What it changes: Its sockets and timeouts fit a context's poll and timers. Built without threads, it could run the 0.6.0 DNS on the application's thread
- What I did: Option 4 of [decision 0005](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0005-dns-resolution.md)

## getaddrinfo_a, GetAddrInfoExW and musl

> November 19, 2026

- Status: Rejected
- Source: [getaddrinfo_a(3)](https://man7.org/linux/man-pages/man3/getaddrinfo_a.3.html), [GetAddrInfoExW](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfoexw) and the [network sources of musl](https://git.musl-libc.org/cgit/musl/tree/src/network)

#### Subject

When an asynchronous resolution completes, glibc notifies the application with a signal or a new thread, and Windows signals an event or calls a routine. musl has no getaddrinfo_a.

- What it changes: None of them gives a socket to poll, so the DNS would differ on each system
- What I did: Dropped from the DNS approaches

## resolv.conf(5)

> November 19, 2026

- Status: Deferred to 0.6.0
- Source: [resolv.conf(5)](https://man7.org/linux/man-pages/man5/resolv.conf.5.html)

#### Subject

glibc waits 5 s for a name server before retrying, and tries twice by default.

- What it changes: A blocking getaddrinfo can freeze a game's loop for seconds, against the README's non-blocking promise
- What I did: Option 1 of [decision 0005](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0005-dns-resolution.md), with a documented exception in the README

## RFC 8305

> November 19, 2026

- Status: Deferred to 0.6.0
- Source: [RFC 8305](https://www.rfc-editor.org/rfc/rfc8305)

#### Subject

Happy Eyeballs starts connecting without waiting for both DNS answers, and staggers its connection attempts by 250 ms.

- What it changes: Connection racing fits one thread with non-blocking connects, a timer and the poll, but a blocking resolution delays it
- What I did: Compared four DNS approaches in [decision 0005](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0005-dns-resolution.md)

## OpenSSL

> November 20, 2026

- Status: Deferred to 0.8.0
- Source: OpenSSL [SSL_get_error](https://docs.openssl.org/3.0/man3/SSL_get_error/), [1.1.1 notes](https://openssl-library.org/news/openssl-1.1.1-notes/index.html), [release strategy](https://openssl-library.org/policies/releasestrat/index.html), [license](https://github.com/openssl/openssl/blob/master/LICENSE.txt), [INSTALL.md](https://github.com/openssl/openssl/blob/master/INSTALL.md) and [OPENSSL_malloc](https://docs.openssl.org/3.5/man3/OPENSSL_malloc/)

#### Subject

On a non-blocking socket, reads, writes and the handshake return `SSL_ERROR_WANT_READ` or `SSL_ERROR_WANT_WRITE` until the socket is ready. `CRYPTO_set_mem_functions()` replaces its allocator for the whole process, and its build needs Perl.

- What it changes: The handshake can progress inside the poll loop without a thread, and its allocations can go through the library's allocator, but it builds outside CMake
- What I did: Option 1 of [decision 0006](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0006-tls-library.md)

## BearSSL

> November 20, 2026

- Status: Deferred to 0.8.0
- Source: BearSSL [API](https://bearssl.org/api1.html), [TLS 1.3 status](https://bearssl.org/tls13.html), [change log](https://bearssl.org/changelog.html) and [license](https://bearssl.org/gitweb/?p=BearSSL;a=blob_plain;f=LICENSE.txt;hb=HEAD)

#### Subject

BearSSL does no I/O and no dynamic allocation, as the caller pushes and pulls plain data and records itself. It has no TLS 1.3 and no release since 0.6 in August 2018.

- What it changes: A TLS decorator could pass its records through any Stream and its memory through the library's allocator, but a security library without fixes for years is a risk
- What I did: Option 2 of [decision 0006](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0006-tls-library.md), with that reserve
