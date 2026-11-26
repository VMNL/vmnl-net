# 0017 System socket

- Date: November 23, 2026
- Status: accepted

**Context and problem**

TCP, UDP and Local embed a system socket, over BSD sockets and Winsock2.
The three systems differ on blocking calls, on inheritance by child processes, on `SIGPIPE` and on the default of `IPV6_V6ONLY`. Each kind of socket could forget a setting, or behave differently on each system.

**Options considered**

1. An internal open that takes a struct of options.
2. An internal open that takes flags, chosen first on November 23.
3. An internal open with fixed settings, and one function per option.

**Option chosen**

Option 3, since November 25.

- The internal open takes the family and the type. A socket is always non-blocking, never inherited by child processes and never raises `SIGPIPE`.
- On Windows, the open creates the socket with `WSASocketW()` and the flags `WSA_FLAG_OVERLAPPED` and `WSA_FLAG_NO_HANDLE_INHERIT`, so the socket is never inheritable, even for an instant.
- The family is explicit. A socket is created as IPv4 or IPv6, and an IPv6 socket clears `IPV6_V6ONLY`, so it also serves IPv4 peers through their mapped address.
- Each option has its own function, like reusing the address.

**Reason**

No caller can forget the fixed settings, and a socket behaves the same on the three systems.
A struct of options was rejected, as every option set before a bind is a boolean. The flags were replaced, as clang-tidy reports the family next to them as parameters easily swapped. A generic setter was rejected too, as it would take the system's constants outside the wrapper and let through options that differ between systems. The wrapper's option functions share one inside its header, which only takes options that behave the same on every system.
Falling back to IPv4 when a host without IPv6 refuses an IPv6 socket, with `EAFNOSUPPORT`, was rejected, as the caller would get another family than the one it asked for. A developer knows the machine their program runs on, and when in doubt, they pick IPv4.
On Windows, creating the socket with `socket()`, then clearing its inheritance with `SetHandleInformation()`, was rejected. A child process created in between could inherit the socket, and a listener created this way accepts inheritable sockets.
A socket that `accept()` returns still goes through `SetHandleInformation()`, as the doc of `accept()` does not say whether it keeps the flag of its listener, though a test showed it does. The [research log](../watch.md) gives the sources.

**Consequences accepted**

- On a host without IPv6, creating an IPv6 socket fails, and an IPv4 socket refuses an IPv6 address.
- An option that differs between systems stays out of the wrapper.
