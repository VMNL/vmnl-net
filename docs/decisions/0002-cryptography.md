# 0002 Cryptography

- Date: November 19, 2026
- Status: accepted, the library itself to choose in [0006](0006-tls-library.md)

**Context and problem**

The 0.8.0 HTTPS client needs TLS over a stream, and the 0.10.0 encrypted traffic needs encryption over datagrams, under RUDP.

**Options considered**

1. Writing the cryptographic primitives and protocols in the library.
2. Wrapping a vetted library, like OpenSSL or BearSSL, behind two decorators. TLS wraps a Stream, and DTLS wraps a Datagram, which RUDP runs over.

**Option chosen**

Option 2.

**Reason**

Cryptography is a specialized field where a single mistake breaks security, so it stays with the libraries written for it.
As decorators, encryption applies to any Stream or Datagram, and RUDP over an encrypted Datagram also protects its headers and acknowledgments.

**Consequences accepted**

- A dependency to choose, build and update, compared in [0006](0006-tls-library.md).
- An encrypted stream is ready when decrypted data is available, not when its socket is, so the poll has to combine what the system reports with what the library knows.
