# 0018 Sends and receives

- Date: November 26, 2026
- Status: accepted

**Context and problem**

The system socket sends and receives buffer views. The system calls behave differently on a call with no view, on a datagram larger than the views, on a half-closed connection and on a send with no address.

**Options considered**

1. Pass each system's behavior through, and document the differences.
2. Give one behavior on every system, and convert where a system differs.

**Option chosen**

Option 2.

- Sends and receives pass the views to the system without a copy, as [decision 0015](0015-buffer-view-layout.md) explains, through `sendmsg()` and `recvmsg()` on POSIX and `WSASendTo()` and `WSARecvFrom()` on Windows. The send and the receive of a connected socket call the datagram functions with no address.
- A call with no view fails with `VMNL_NET_EINVAL`. Linux returned 0 bytes with success, like a closed connection, while macOS failed with `EMSGSIZE` and Windows with `WSAEINVAL`.
- A datagram larger than the views fails with `VMNL_NET_EMSGSIZE`, and its end is lost. POSIX returned the cut length with success and only raised `MSG_TRUNC`.
- The shutdown only closes the sending side, with `SHUT_WR` or `SD_SEND`, so the peer reads the bytes sent, then the end of the stream.
- `EDESTADDRREQ` and `WSAEDESTADDRREQ` translate to `VMNL_NET_ENOTCONN`, the code that Windows already gives for a send with no address on a socket that is not connected.

**Reason**

An application writes one code path, and the same test gives the same results on Linux, macOS and Windows.
A cut length with success everywhere was rejected, as the application could not tell a cut datagram from a whole one. Closing the receiving side was rejected, as Windows resets the connection when bytes arrive after `SD_RECEIVE`, unlike POSIX.

**Consequences accepted**

- A datagram socket never receives part of a datagram.
- The receiving side stays open until the socket closes.
- A send of more than `INT_MAX` bytes fails with `VMNL_NET_EINVAL` on macOS, and a send of 2 GiB with `VMNL_NET_ENOBUFS` on Windows, until the streams cap their sends.
- Batching with `recvmmsg()` or Registered I/O is deferred to 0.6.0, as the [research log](../watch.md) explains.

**Completed on November 27, 2026**

- A send to an address on a connected datagram socket fails with `VMNL_NET_EISCONN`. macOS refused it, while Linux and Windows sent the datagram to the address. The system socket remembers a successful connect, so the check costs no system call.
- A send of an empty datagram returns 0 with success, and the receiver gets an empty datagram, on every system.
- A send or a receive takes at most `VMNL_NET_SOCKET_MAX_VIEWS` views, 1024, the IOV_MAX of Linux and macOS, and fails with `VMNL_NET_EMSGSIZE` above it. Windows took more views.
- Views of more than INT_MAX bytes fail with `VMNL_NET_EINVAL`, which only macOS checked. The check adds the view sizes before each call.
- A receive on an unbound socket fails with `VMNL_NET_EAGAIN`, as on POSIX, where Windows failed with WSAEINVAL.
