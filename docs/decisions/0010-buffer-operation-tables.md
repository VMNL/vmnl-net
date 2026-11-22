# 0010 Buffer operation tables

- Date: October 21, 2026
- Status: accepted

**Context and problem**

Version 0.2.0 had dynamic and ring buffers behind one API, and each function checked the buffer's type.
Version 0.3.0 adds flat and chunked buffers, which would add a branch to every function.

**Options considered**

1. One API per kind of buffer.
2. One API that checks the buffer's type in each function, like 0.2.0.
3. One API over a table of operations per kind of buffer.

**Option chosen**

Option 3. Each kind fills a table with its operations, and the public functions call through it.
An operation shared by several kinds is written once and referenced from their tables.

**Reason**

The application changes a buffer's kind without changing its code, and a new kind adds a table instead of a branch in every function.

**Consequences accepted**

- Each operation costs an indirect call.
- The socket families of [0001](0001-socket-model.md) reuse the same pattern.
