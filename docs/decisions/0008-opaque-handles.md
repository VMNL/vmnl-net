# 0008 Opaque handles

- Date: October 21, 2026
- Status: accepted

**Context and problem**

Other languages call the library through its C ABI, which has to stay stable for 1.0.0.
The public types decide who owns their memory and how their layout can change.

**Options considered**

1. Public structures that the application embeds or allocates itself.
2. Objects created in memory the caller gives, the stack included.
3. Opaque handles that the library creates and destroys, with fixed-size integers in the signatures.

**Option chosen**

Option 3.

**Reason**

A structure can change behind its handle without breaking the ABI, and bindings stay simple, like a `ctypes` pointer in Python or a `NonNull` with `Drop` in Rust.
The library owns every byte it exposes, so no free or lifetime question is left to the caller.
Option 2 mixes the caller's memory with the library's, which raises exactly those questions.

**Consequences accepted**

- Each object costs an allocation.
- Small transient data stays in public structures, like the buffer views, the addresses and the options.
- Zero allocation on the hot path would come later from pools owned by the context, once the benchmarks show the need.
