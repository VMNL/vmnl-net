# 0002 Cryptography

- Date: 2026-11-19
- Status: accepted, the library itself to choose for 0.8.0

**Context and problem**

The HTTPS client of 0.8.0 needs TLS over a stream, and the encrypted traffic of 0.10.0 needs encryption over datagrams, under RUDP.

**Options considered**

1. Writing the cryptographic primitives and the protocols in the library.
2. Wrapping a vetted library, such as OpenSSL or BearSSL, behind decorators: one on Stream, like TLS, and one on Datagram, like DTLS, which RUDP runs over.

**Option chosen**

Option 2.

**Reason**

Cryptography is a specialized field where a single mistake breaks the security, so it stays with the libraries written for it.
As decorators, the encryption applies to any Stream or Datagram, and RUDP over an encrypted Datagram also protects its headers and its acknowledgments.

**Consequences accepted**

- A dependency to choose, build and update: OpenSSL is maintained but under Apache-2.0 with a build that needs Perl, and BearSSL fits the library but has no TLS 1.3 and no release since 2018, as logged in the [research log](../watch.md).
- An encrypted stream is ready when decrypted data is available, not when its socket is, so the poll has to combine what the system reports and what the library knows.
