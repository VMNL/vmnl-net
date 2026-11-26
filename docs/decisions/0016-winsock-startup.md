# 0016 Winsock startup

- Date: November 23, 2026
- Status: accepted

**Context and problem**

Windows needs `WSAStartup()` before any Winsock call, and `WSACleanup()` after the last one. It counts the calls, so each startup needs its own cleanup.
Linux and macOS need nothing before their first socket.

**Options considered**

1. A public init and shutdown pair, called once by the application.
2. Each context starts Winsock when it is created, and stops it when it is destroyed.

**Option chosen**

Option 2. On Windows, `vmnl_net_context_create()` calls `WSAStartup()`, and `vmnl_net_context_destroy()` calls `WSACleanup()`.

**Reason**

A public pair would do nothing on Linux and macOS, and an application could forget it. A context already exists before any socket, and the counted calls let several contexts live side by side.

**Consequences accepted**

- A socket is created in a context and must not outlive the last one.
- Creating a context can fail on Windows, with `VMNL_NET_FSYSTEM`.
