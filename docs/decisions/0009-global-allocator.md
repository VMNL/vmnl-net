# 0009 Global allocator

- Date: October 14, 2026
- Status: accepted

**Context and problem**

The application can replace the library's `malloc`, `realloc` and `free`.
The first context came with one global allocator on October 4, 2026, then the allocator moved into the context on October 8, 2026.
Every function that allocated then needed a context, the buffers included, so the context spread into signatures that had nothing to do with it.

**Options considered**

1. An allocator given to each context.
2. An allocator stored in each object.
3. An allocator or a context passed to each function that allocates.
4. One global allocator that the application replaces, like `uv_replace_allocator()` in libuv.

**Option chosen**

Option 4. `vmnl_net_allocator_set()` replaces it for the whole library.

**Reason**

No signature carries an allocator, and the buffer functions no longer take a context.
Two users of the library in one process would share the allocator, which is rare.

**Consequences accepted**

- The allocator is replaced before any object exists or once none is alive.
- The allocator has to be thread-safe, since every context uses it.
