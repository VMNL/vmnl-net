# 0015 Buffer view layout

- Date: November 26, 2026
- Status: accepted, to be implemented in 0.3.0

**Context and problem**

The buffers give their bytes as views, which the sockets pass to the system without copying the bytes.
On POSIX, `sendmsg()` and `recvmsg()` take an array of `struct iovec`, a pointer then a `size_t`. A view has the same layout, so the array is cast.
On Windows, `WSASend()` and `WSARecv()` take an array of `WSABUF`, a `ULONG` length then a pointer. A view does not match it, so each call would copy the views into a `WSABUF` array.
Windows sets no limit like `IOV_MAX` on that array, so a bounded copy would also split a send into batches.

**Options considered**

1. One layout on every system, with a copy into `WSABUF` on Windows.
2. A layout per system with the same field names, which matches `struct iovec` on POSIX and `WSABUF` on Windows, like libuv's `uv_buf_t`.

**Option chosen**

Option 2. On Windows, a view holds a `uint32_t` size then the data pointer. Elsewhere, it holds the data pointer then a `size_t` size.
`VmnlNetBufferViewSize` and `VMNL_NET_BUFFER_VIEW_SIZE_MAX` give the size's type and its maximum, so the buffers' code needs no `#ifdef`.
The socket wrapper casts an array of views to the system's array, and static assertions in its header check the sizes and offsets of both types.
A typedef of `struct iovec` or `WSABUF` was rejected. The field names would change between systems, and `buffer.h` would include the system headers.

**Reason**

The views reach the system without a copy on every platform, and `buffer.h` still includes no system header.

**Consequences accepted**

- A view holds at most `VMNL_NET_BUFFER_VIEW_SIZE_MAX` bytes, `UINT32_MAX` on Windows, so a buffer splits a larger span into several views there.
- The size field has a different type on Windows and elsewhere.
- The fields come in a different order, so a view is initialized by field names, never by position.
