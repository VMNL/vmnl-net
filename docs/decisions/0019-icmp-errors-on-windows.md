# 0019 ICMP errors on Windows

- Date: November 26, 2026
- Status: accepted

**Context and problem**

A datagram sent to a closed port draws an ICMP port unreachable. Linux and macOS report it at the next receive of a connected socket only, as `ECONNREFUSED`.
Windows reports it on every UDP socket, connected or not, as `WSAECONNRESET`, and also reports an expired time to live as `WSAENETRESET`. A Windows server then fails its next receive whenever a client disappears.

**Options considered**

1. Ignore `WSAECONNRESET` at each receive, like libuv and ENet.
2. Turn both reports off on every UDP socket, with `SIO_UDP_CONNRESET` and `SIO_UDP_NETRESET`, like Go.
3. Turn the port unreachable off when the socket is created, and on again after a connect, like OpenJDK.
4. Never report a port unreachable on any system, and filter `ECONNREFUSED` on POSIX.

**Option chosen**

Option 3, with the expired time to live of option 2. The internal open clears `SIO_UDP_CONNRESET` and `SIO_UDP_NETRESET`, and the connect sets `SIO_UDP_CONNRESET` again. A `WSAECONNRESET` on a datagram socket translates to `VMNL_NET_ECONNREFUSED`.

**Reason**

POSIX cannot turn the report off, so only this option gives the same behavior on every system. A connected client learns at once that the port is closed, instead of waiting for a timeout. It costs one `WSAIoctl()` at the open, and one at the connect.
Options 1 and 2 would hide the error of a connected socket on Windows only. Option 4 would lose that early warning everywhere.

**Consequences accepted**

- An unconnected datagram socket never learns that a port is closed.
- A forged ICMP message can fail the receive of a connected socket, on every system.
- The drop of an expired time to live is not tested, as it needs a router on the path.

The sources and the tests are in the [research log](../watch.md).
