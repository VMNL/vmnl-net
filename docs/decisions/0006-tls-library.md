# 0006 TLS library

- Date: November 22, 2026
- Status: proposed, to decide for 0.8.0

**Context and problem**

Decision [0002](0002-cryptography.md) wraps a vetted library behind two decorators, TLS over a Stream for the 0.8.0 HTTPS client and DTLS over a Datagram for the 0.10.0 encrypted traffic, but leaves the library to choose.
The choice rests on TLS 1.3, maintenance, license, allocations and build.
The handshake has to progress inside the application's poll, since the library runs no thread of its own.

**Options considered**

1. OpenSSL, under Apache-2.0, with TLS 1.3 since 1.1.1 and a 3.5 LTS maintained until April 8, 2030. On a non-blocking socket, its reads, writes and handshake return `SSL_ERROR_WANT_READ` or `SSL_ERROR_WANT_WRITE` until the socket is ready. `CRYPTO_set_mem_functions()` can send its allocations through the library's allocator, for the whole process. Its build needs Perl and runs outside CMake.
2. BearSSL, under MIT, an engine with no I/O and no dynamic allocation. The caller pushes and pulls plain data and records over four channels, so a decorator could pass its records through any Stream. It has no TLS 1.3 and no release since 0.6 on August 14, 2018, a risk for a security library.

**Option chosen**

None yet, to choose before the 0.8.0 HTTPS client.
Neither library's DTLS support is checked yet, and no other library is compared yet.

The sources are in the [research log](../watch.md).
