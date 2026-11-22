# Watch

This file keeps track of every watch this library's decisions are built on, see [references](references.md) for papers and articles.

## The intrusive list of Linux

> October 12, 2026

- Status: Adopted in [6aa55e1](https://github.com/VMNL/vmnl-net/commit/6aa55e1)
- Source: The intrusive list of Linux, [include/linux/list.h](https://github.com/torvalds/linux/blob/master/include/linux/list.h)

#### Subject

A generic doubly linked list whose links live inside the structures they chain.

- What it changes: One list serves every module that chains structures, without allocating per link
- What I did: Ported it as the library's intrusive list, first for a context's active timers sorted by deadline, now also for the chunked buffer's chunks

## Apple App Review Guidelines, rule 2.5.5

> November 19, 2026

- Status: Planned
- Source: Apple [App Review Guidelines](https://developer.apple.com/app-store/review/guidelines/), rule 2.5.5, [IPv6-only networks](https://developer.apple.com/support/ipv6/) and [Supporting IPv6 DNS64/NAT64 Networks](https://developer.apple.com/library/archive/documentation/NetworkingInternetWeb/Conceptual/NetworkingOverview/UnderstandingandPreparingfortheIPv6Transition/UnderstandingandPreparingfortheIPv6Transition.html)

#### Subject

The App Store requires apps to work on IPv6-only networks, where getaddrinfo turns an IPv4 literal into a synthesized IPv6 address.

- What it changes: inet_pton keeps an IPv4 literal as an IPv4-mapped address, which reaches nothing on such a network
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
- What I did: The timer benchmarks rule out the sorted list for deadlines out of order, so a wheel is to prototype against a heap

## Hashed and Hierarchical Timing Wheels

> October 17, 2026

- Status: Planned
- Source: [Hashed and Hierarchical Timing Wheels](http://www.cs.columbia.edu/~nahum/w6998/papers/sosp87-timing-wheels.pdf) by George Varghese and Tony Lauck (SOSP, 1987)

#### Subject

A timer wheel starts and stops a timer in constant time, where a sorted list has to find the place of each new deadline.

- What it changes: The library's sorted list searches from its back, so it can walk all its timers on each start unless their timeouts are equal
- What I did: The benchmarks confirmed it on the reference machine. A start costs 3.3 ns with deadlines in order, but 19 to 21 µs out of order with 32,000 active timers in a new process, up to 26 µs after repeated runs in the same process, so the 1,000 starts of a frame take longer than a 60 Hz frame. A wheel and a heap come next, measured with the same benchmarks

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
- What I did: A blocking resolution would need a documented exception in the README

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
