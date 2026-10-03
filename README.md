<div align="center">

# VMNL / Network

**Networking library in C**

TCP, raw UDP, reliable UDP and socket polling for games and real-time applications.

<br>

![C11](https://img.shields.io/badge/C11-A8B9CC?style=for-the-badge&logo=c&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.28%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge)
![macOS](https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=apple&logoColor=white)

<br>

[![CI](https://img.shields.io/github/actions/workflow/status/VMNL/vmnl-net/ci.yml?style=flat-square&label=CI)](https://github.com/VMNL/vmnl-net/actions/workflows/ci.yml)
![License](https://img.shields.io/github/license/VMNL/vmnl-net?style=flat-square)
![Repo size](https://img.shields.io/github/repo-size/VMNL/vmnl-net?style=flat-square)
![Stability](https://img.shields.io/badge/stability-experimental-orange?style=flat-square)

</div>

---

> [!WARNING]
> **In development, not released yet.**
> Most features are still planned and the API can change anytime until `1.0.0`.
>
> **Do not** use it in production yet.


## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Status](#status)
- [Installation](#installation)
- [Technical Documentation](#technical-documentation)
- [Roadmap](#roadmap)
- [References](#references)
- [Author](#author)
- [License](#license)


## Overview

`vmnl_net` is a cross-platform networking library written in C11.
It exposes a single non-blocking API over the native socket interfaces of Linux, macOS and Windows, and builds higher-level protocols on top of it.
It is developed as the networking module of [VMNL](https://github.com/VMNL/vmnl) and can be used on its own from any C or C++ project.

Feature highlights:

- TCP, raw UDP and a reliable UDP protocol with connections and channels
- Native readiness polling: epoll on Linux, kqueue on macOS and BSD, WSAPoll on Windows
- IPv4 and IPv6
- No background threads: the application drives the library from its own loop
- Explicit resource management: opaque handles, custom allocator and logging hooks, errors returned as codes
- An ABI designed to be wrapped from other languages, with fixed-size types and versioned structures


## Architecture

TODO: describe the modules and how they depend on each other once the first ones are in place.


## Status

`vmnl_net` is under active development and has no release yet.
Features are grouped by milestone: **Beta** lists what the beta release delivers, **Release** what follows up to `1.0.0`.
The target column refers to the [Roadmap](#roadmap).

A feature moves from *Planned* to *In progress* when work starts, and to *Done* once it is implemented, documented and tested on every Tier 1 platform.

### Beta

| Feature | Scope | Target | Status |
| --- | --- | --- | --- |
| Core | Library context, error codes, custom allocator and logging hooks | `0.1.0` | In progress |
| Buffers | Dynamic and circular byte buffers | `0.2.0` | Planned |
| Timers | Monotonic clock and application-driven timers | `0.2.0` | Planned |
| Socket wrapper | Non-blocking abstraction over BSD sockets and Winsock2 | `0.3.0` | Planned |
| TCP | Stream sockets: connect, listen, accept, send, receive | `0.3.0` | Planned |
| Raw UDP | Datagram sockets: send to and receive from any address | `0.3.0` | Planned |
| Serialization | Bit packing, fixed-size and variable-length integers, with bounds checking | `0.4.0` | Planned |
| Connections | Reliable UDP handshake, keepalive, timeouts and disconnection events | `0.5.0` | Planned |
| Unreliable channels | Sequenced delivery; out-of-date messages are discarded | `0.5.0` | Planned |
| Reliable channels | Ordered delivery with acknowledgements and retransmission | `0.5.0` | Planned |
| Statistics | Per-connection round-trip time and packet loss | `0.5.0` | Planned |
| Polling | Readiness notification for many sockets (see [Platforms](#platforms)) | `0.6.0` | Planned |
| DNS | Host name resolution to IPv4 and IPv6 addresses | `0.6.0` | Planned |

### Release

| Feature | Scope | Target | Status |
| --- | --- | --- | --- |
| Fragmentation | Reliable messages larger than the path MTU | `0.7.0` | Planned |
| HTTP | HTTP/1.1 client ([RFC 9112](https://www.rfc-editor.org/rfc/rfc9112)) | `0.8.0` | Planned |
| HTTPS | HTTP over TLS | `0.8.0` | Planned |
| Text chat | Text chat primitives over reliable channels | `0.9.0` | Planned |
| Voice chat | Voice chat primitives over unreliable channels | `0.9.0` | Planned |
| Packet encryption | Encrypted traffic between clients and servers | `0.10.0` | Planned |

### Platforms

| Platform | Polling API | Tier |
| --- | --- | --- |
| Linux | epoll | 1 |
| macOS | kqueue | 1 |
| Windows | WSAPoll | 1 |
| FreeBSD, OpenBSD, NetBSD | kqueue | 2 |


Tier 1 platforms are built and tested by the CI on every change.
Tier 2 platforms share code with a Tier 1 platform and are expected to work, but are not tested.


## Installation

### From GitHub releases

Each release will ship prebuilt archives for Linux, macOS and Windows, with the headers, static and shared libraries.
They will be on the [releases page](https://github.com/VMNL/vmnl-net/releases).

There is no release yet.

### From sources

`vmnl_net` is written in C11.
You need CMake 3.28 or later and one of these compilers:

- GCC 4.9 or later;
- Clang 3.3 or later, or Apple Clang;
- MSVC from Visual Studio 2019 version 16.8 or later, with the Windows SDK 10.0.20348.0 or later.

These are the first versions with C11 support.
The CI builds with the current ones.

The build system is not in the repository yet: these commands will work once it lands.

```sh
git clone https://github.com/VMNL/vmnl-net.git
cd vmnl-net
cmake -B build
cmake --build build --config Release
cmake --install build --config Release --prefix <install directory>
```

Once installed, CMake projects can find the library with `find_package(vmnl_net)`, and other build systems through `pkg-config vmnl-net`.


## Technical Documentation

The CI generates the API reference with Doxygen from the public headers and publishes it on [GitHub Pages](https://vmnl.github.io/vmnl-net/).
It goes online with the first module.

The `docs` folder also has:

- [API](docs/api.md): every public function, grouped by module;
- [Architecture](docs/architecture.md): the modules and how they fit together.

Both are still in progress.


## Roadmap

Target dates are given by month and updated at each release.

| Version | Content | Target |
| --- | --- | --- |
| `0.1.0` | Build system, CI, packaging and core | October 2026 |
| `0.2.0` | Buffers and timers | November 2026 |
| `0.3.0` | Socket wrapper, TCP and raw UDP | December 2026 |
| `0.4.0` | Serialization | January 2027 |
| `0.5.0` | Reliable UDP | March 2027 |
| `0.6.0` | Polling and DNS; the last `0.6.x` is the beta | April 2027 |
| `0.7.0` | Fragmentation | June 2027 |
| `0.8.0` | HTTP and HTTPS | October 2027 |
| `0.9.0` | Text and voice chat | November 2027 |
| `0.10.0` | Packet encryption | December 2027 |
| `1.0.0` | First stable release, with a stable C ABI | February 2028 |


## References

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [Gaffer On Games, game networking articles](https://gafferongames.com/categories/game-networking/)
- [netcode](https://github.com/mas-bandwidth/netcode), secure client/server connections over UDP
- [Quake 3 Source Code Review: Network Model](https://fabiensanglard.net/quake3/network.php) by Fabien Sanglard
- [Quake III Arena network channel](https://github.com/id-Software/Quake-III-Arena/blob/master/code/qcommon/net_chan.c): snapshots, delta compression, sequencing and fragmentation over UDP
- [RFC 768](https://www.rfc-editor.org/rfc/rfc768) (UDP), [RFC 9293](https://www.rfc-editor.org/rfc/rfc9293) (TCP) and [RFC 9112](https://www.rfc-editor.org/rfc/rfc9112) (HTTP/1.1)
- [epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html), [kqueue(2)](https://man.freebsd.org/cgi/man.cgi?query=kqueue) and [WSAPoll](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll)


## Author

[Laszlo Serdet](https://github.com/lszsrd), networking lead of VMNL.


## License

Released under the [MIT License](LICENSE).
