# 0005 DNS resolution

- Date: November 22, 2026
- Status: proposed, to decide for 0.6.0

**Context and problem**

Version 0.6.0 resolves host names to IPv4 and IPv6 addresses, and no library function may block the application's thread.
The system's `getaddrinfo()` blocks. glibc waits 5 s for a name server before retrying, and tries twice by default.
Happy Eyeballs ([RFC 8305](https://www.rfc-editor.org/rfc/rfc8305)) races connections with non-blocking connects, a timer and the poll, which all fit one thread, but a blocking resolution delays the race.
The App Store requires apps to work on IPv6-only networks, where `getaddrinfo()` turns an IPv4 literal into a synthesized IPv6 address. So a remote peer's address goes through the resolution even when it is a numeric IP.

**Options considered**

1. The system's blocking `getaddrinfo()`, the same everywhere, with a documented exception to the non-blocking rule, since a call can stall the application's loop for seconds.
2. A stub resolver in the library, over UDP with a TCP fallback ([RFC 1035](https://www.rfc-editor.org/rfc/rfc1035), [RFC 3596](https://www.rfc-editor.org/rfc/rfc3596) and [RFC 7766](https://www.rfc-editor.org/rfc/rfc7766)), built on the datagram sockets, the poll, the timers and the serialization. It has to read each system's settings, since macOS ignores `/etc/resolv.conf`, read the hosts file and cache answers for their TTL. It does without mDNS, VPN split DNS and the system's encrypted DNS, and synthesizing IPv6 addresses for IPv4 literals needs [RFC 7050](https://www.rfc-editor.org/rfc/rfc7050) and [RFC 6052](https://www.rfc-editor.org/rfc/rfc6052).
3. Each system's asynchronous resolution, rejected. glibc's `getaddrinfo_a()` notifies with nothing, a signal or a new thread, musl lacks it, and the `GetAddrInfoExW()` of Windows signals an event or calls a routine, so none gives a socket to poll.
4. c-ares, a DNS stub resolver under MIT. Its sockets join the poll through a callback, `ares_process_fds()` (1.34.0) handles the ready ones and `ares_timeout()` gives the timers the next deadline. It reads the Linux, macOS and Windows settings. It is a dependency to vendor and to build without its threads, so its settings reload by hand after a network change, and it does not synthesize IPv6 addresses for IPv4 literals.

**Option chosen**

None yet, to choose before the 0.6.0 resolution.
Whatever the option, the resolution stays behind a library handle, so the option can change without breaking the API.

The sources are in the [research log](../watch.md).
