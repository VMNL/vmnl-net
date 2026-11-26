# Architecture

vmnl-net is a C11 library linked into the application, statically or as a shared library.
It runs no service and no thread of its own. Every function runs inside an application call, on the thread that owns the context.

## Layers

The library is built in layers, each module pointing to the ones it uses.
In the diagram, solid lines show what exists and dotted ones what is planned.

Every module reports its failures with errors, so those arrows are left out.
RUDP is a connection over UDP with reliable and unreliable channels.
Text chat uses the reliable ones, voice chat the unreliable ones, without retransmission.

```mermaid
%%{init: {"theme": "neutral"}}%%
block-beta
    columns 1

    block:SERVICES
        columns 10
        SERVICES_TITLE["Services"]
        HTTP["<sup>0.8</sup> HTTP(S) client"]
        space:5
        CHAT["<sup>0.9</sup> text chat"]
        VOICE["<sup>0.9</sup> voice chat"]
        space
    end

    block:PROTOCOLS
        columns 10
        PROTOCOLS_TITLE["Protocols"]
        TCP["<sup>0.3</sup> TCP"]
        space:2
        CRYPT["<sup>0.10</sup> encryption"]
        space
        UDP["<sup>0.3</sup> UDP"]
        RUDP["<sup>0.5</sup> RUDP"]
        FRAG["<sup>0.7</sup> fragmentation"]
        space
    end

    block:IO
        columns 10
        IO_TITLE["I/O"]
        SOCKET["<sup>0.3</sup> sockets"]
        space:4
        POLL["<sup>0.6</sup> polling"]
        space
        DNS["<sup>0.6</sup> DNS resolution"]
        space
    end

    block:DATA
        columns 10
        DATA_TITLE["Data"]
        BUFFER["<sup>0.2</sup> buffer API"]
        space:3
        SER["<sup>0.4</sup> serialization"]
        space:4
    end

    block:BUFFERS
        columns 10
        BUFFERS_TITLE["Buffers"]
        FLAT["<sup>0.3</sup> flat"]
        RING["<sup>0.2</sup> ring"]
        DYNAMIC["<sup>0.2</sup> dynamic"]
        CHUNKED["<sup>0.3</sup> chunked"]
        space:5
    end

    block:TIME
        columns 10
        TIME_TITLE["Time"]
        space:6
        TIMER["<sup>0.2</sup> timers"]
        CLOCK["<sup>0.2</sup> clock"]
        space
    end

    block:CORE
        columns 10
        CORE_TITLE["Core"]
        space
        ALLOCATOR["<sup>0.1</sup> allocator"]
        space:3
        CONTEXT["<sup>0.1</sup> context"]
        ERROR["<sup>0.1</sup> errors"]
        VERSION["<sup>0.1</sup> version"]
        space
    end

    block:INTERNAL
        columns 10
        INTERNAL_TITLE["Internal"]
        space:4
        LIST["<sup>0.2</sup> intrusive list"]
        space:4
    end

    HTTP -.-> TCP
    CHAT -.-> RUDP
    VOICE -.-> RUDP
    RUDP -.-> UDP
    RUDP -.-> FRAG
    RUDP -.-> TIMER
    TCP -.-> SOCKET
    UDP -.-> SOCKET
    SOCKET -.-> POLL
    SOCKET -.-> BUFFER
    SER -.-> BUFFER
    BUFFER --> FLAT
    BUFFER --> DYNAMIC
    BUFFER --> RING
    BUFFER --> CHUNKED
    POLL -.-> CONTEXT
    TIMER --> CONTEXT
    TIMER --> ALLOCATOR
    TIMER --> LIST
    CONTEXT --> TIMER
    CONTEXT --> CLOCK
    CONTEXT --> ALLOCATOR
    CONTEXT --> LIST
    FLAT --> ALLOCATOR
    DYNAMIC --> ALLOCATOR
    RING --> ALLOCATOR
    CHUNKED --> ALLOCATOR
    CHUNKED --> LIST

    classDef default stroke:#6f7e8e
    classDef title fill:none,stroke:none
    classDef planned stroke:#6f7e8e,stroke-dasharray: 4 3

    class SERVICES_TITLE,PROTOCOLS_TITLE,IO_TITLE,DATA_TITLE,BUFFERS_TITLE,TIME_TITLE,CORE_TITLE,INTERNAL_TITLE title
    class HTTP,CHAT,VOICE,TCP,UDP,RUDP,FRAG,CRYPT,SOCKET,POLL,DNS,SER planned
```

The application gives the time to its context, so the timers never read the clock themselves.
The bytes never go through a library copy on their way in or out. The application writes into the free space a buffer exposes and reads the stored bytes in place.

## Modules

### allocator

> 0.1

- Uses: the C runtime

One global allocator, replaceable by the application, with `malloc`, `realloc` and `free` by default.

### errors

> 0.1

The error codes every function reports, and their messages.

### version

> 0.1

The version of the library linked at run time.

### context

> 0.1

- Uses: allocator, clock, timers, list

The state of one thread, with its active timers and the time given to its last advance.

### clock

> 0.2

- Uses: `clock_gettime()` on POSIX, `QueryPerformanceCounter()` on Windows

A monotonic clock in nanoseconds.

### list

> 0.2

An internal generic intrusive doubly linked list.

### timers

> 0.2, reworked in 0.3

- Uses: context, allocator, list

One-shot and periodic timers, fired by the time the application gives to its context.

### buffers

> 0.2 and 0.3

- Uses: allocator, list

Byte queues written and read in place, as flat, ring, dynamic or chunked buffers behind one API.

The planned modules follow the [roadmap](../README.md#roadmap) and appear dotted in the diagram.

## Dependencies

The library has no third-party dependency. It only needs the C runtime and the system sockets, Winsock2 on Windows and BSD sockets elsewhere.
The first ones may come with the DNS resolution of 0.6 and the TLS library of 0.8, like c-ares or OpenSSL, still to choose in decisions [0005](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0005-dns-resolution.md) and [0006](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0006-tls-library.md).
The build and development tools are listed in [docs/build.md](build.md).

## Deployment

The library ships as static and shared archives for Windows, macOS and Linux on x86_64 and arm64, with a CMake package and a pkg-config file.
It can also be built from its sources, with `FetchContent` for instance.
It lives in the application's process, and the application owns the loop. A context belongs to one thread at a time, and the allocator is global to the process.

## Points of failure

### An allocation fails

The function reports `VMNL_NET_ENOMEM`.

### A buffer lacks free space

A flat or ring buffer makes `vmnl_net_buffer_prepare()` report `VMNL_NET_ENOBUFS`.
A dynamic buffer grows by 1.5 times, and a chunked buffer adds a 16 KiB chunk without moving its bytes.

### The application falls behind

The timers fire late, and each callback learns how late it runs.
A periodic timer that missed several deadlines fires once.

### The number of active timers grows

Starting a timer can walk the whole sorted list of [decision 0011](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0011-sorted-list-of-timers.md), so its worst cost grows with the number of active timers, while equal timeouts still insert at once from the back.
On the reference machine, a start costs 3.3 ns with deadlines in order, and 19 to 21 µs out of order with 32,000 active timers in a new process, up to 26 µs after repeated runs in the same process.

### Two threads use one context

A data race, since the context has no lock. Its documentation forbids it.

### The allocator is replaced while objects are alive or other threads run

Memory freed by the wrong allocator, or a data race. Its documentation forbids it.

## Structuring choices

- [0001 Socket model](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0001-socket-model.md)
- [0002 Cryptography](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0002-cryptography.md)
- [0005 DNS resolution](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0005-dns-resolution.md), proposed
- [0006 TLS library](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0006-tls-library.md), proposed
- [0007 No background thread](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0007-no-background-thread.md)
- [0008 Opaque handles](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0008-opaque-handles.md)
- [0009 Global allocator](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0009-global-allocator.md)
- [0010 Buffer operation tables](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0010-buffer-operation-tables.md)
- [0011 Sorted list of timers](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0011-sorted-list-of-timers.md)
- [0012 Timer structure](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0012-timer-structure.md), proposed
- [0014 Polling](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0014-polling.md), proposed
- [0015 Buffer view layout](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0015-buffer-view-layout.md)
- [0016 Winsock startup](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0016-winsock-startup.md)
- [0017 System socket](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0017-system-socket.md)
- [0018 Sends and receives](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0018-sends-and-receives.md)
- [0019 ICMP errors on Windows](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0019-icmp-errors-on-windows.md)

## Data model

```mermaid
%%{init: {"theme": "neutral"}}%%
erDiagram
    ALLOCATOR ||--o{ CONTEXT : "allocates"
    ALLOCATOR ||--o{ TIMER : "allocates"
    ALLOCATOR ||--o{ BUFFER : "allocates"
    CONTEXT ||--o{ TIMER : "keeps active, sorted by deadline"
    BUFFER ||--o{ CHUNK : "chains, for a chunked buffer"
    BUFFER ||--o{ VIEW : "exposes for a write or a read"
```

- allocator, alive for the whole process, made of 3 function pointers
- context, created and destroyed by the application, one per thread, holding its active timers and the time of its last advance
- timer, created by the application and started in a context, holding one link, a callback and its data, a deadline and an interval
- buffer, created and destroyed by the application, with a capacity fixed for flat and ring buffers
- chunk, owned by a chunked buffer, holding 16 KiB of bytes
- view, given by a buffer for a write or a read and invalid once the buffer is destroyed, made of a pointer and a size

The expected numbers of timers and buffers per context will be measured once the sockets exist.
