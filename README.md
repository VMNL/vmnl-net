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


## Installation

### From GitHub releases

Each release will ship two prebuilt archives for Linux, macOS and Windows: one with the static library and one with the shared library, both with the headers.
They will be on the [releases page](https://github.com/VMNL/vmnl-net/releases).

There is no release yet.

### From sources

`vmnl_net` is written in C11.
You need CMake 3.28 or later and one of these compilers:

- GCC 4.9 or later;
- Clang 3.3 or later, or Apple Clang;
- MSVC from Visual Studio 2019 version 16.8 or later, with the Windows SDK 10.0.20348.0 or later.

These are the first versions with C11 support.

> [!NOTE]
> Their support is theoretical: the CI only builds and tests with the compilers of the latest GitHub runners.

```sh
git clone https://github.com/VMNL/vmnl-net.git
cd vmnl-net
cmake -B build
cmake --build build --config Release
cmake --install build --config Release --prefix <install directory>
```

The build can be tuned with these CMake options:

| Option | Default | Effect |
| --- | --- | --- |
| `BUILD_SHARED_LIBS` | `OFF` | Builds a shared library instead of a static one |
| `VMNL_NET_BUILD_TESTS` | `ON` when built on its own | Builds the tests |
| `VMNL_NET_BUILD_DOCS` | `OFF` | Generates the documentation with Doxygen into `build/docs/html` |
| `VMNL_NET_INSTALL` | `ON` when built on its own | Generates the install rules |

The tests are built with the library when it is built on its own, and run with:

```sh
ctest --test-dir build --build-config Release
```


## Technical Documentation

The CI generates the API reference with Doxygen from the public headers and publishes it on [GitHub Pages](https://vmnl.github.io/vmnl-net/).
It goes online with the first module.

The `docs` folder also has:

- [API](docs/api.md): every public function, grouped by module;
- [Architecture](docs/architecture.md): the modules and how they fit together.

Both are still in progress.


## Roadmap

Target dates are given by month and updated at each release.
A version is *Planned* until work starts, *In progress* while it is built, and *Released* once it is implemented, documented and tested on Linux, macOS and Windows.

| Version | Content | Target | Status |
| --- | --- | --- | --- |
| `0.1.0` | Build system, CI and packaging; core: library context, error codes, custom allocator and logging hooks | October 2026 | In progress |
| `0.2.0` | Dynamic and circular byte buffers; monotonic clock and application-driven timers | November 2026 | Planned |
| `0.3.0` | Non-blocking socket wrapper over BSD sockets and Winsock2, TCP stream sockets and raw UDP datagram sockets | December 2026 | Planned |
| `0.4.0` | Serialization: bit packing, fixed-size and variable-length integers, with bounds checking | January 2027 | Planned |
| `0.5.0` | Reliable UDP: connections, sequenced unreliable channels, ordered reliable channels with acknowledgements and retransmission, round-trip time and packet loss statistics | March 2027 | Planned |
| `0.6.0` | Polling for many sockets with epoll, kqueue or WSAPoll and DNS resolution to IPv4 and IPv6; the last `0.6.x` is the beta | April 2027 | Planned |
| `0.7.0` | Fragmentation of reliable messages larger than the path MTU | June 2027 | Planned |
| `0.8.0` | HTTP/1.1 client ([RFC 9112](https://www.rfc-editor.org/rfc/rfc9112)) and HTTPS | October 2027 | Planned |
| `0.9.0` | Text chat over reliable channels and voice chat over unreliable channels | November 2027 | Planned |
| `0.10.0` | Encrypted traffic between clients and servers | December 2027 | Planned |
| `1.0.0` | First stable release, with a stable C ABI | February 2028 | Planned |


## References

- [Gaffer On Games, game networking articles](https://gafferongames.com/categories/game-networking/) by Glenn Fiedler: my main source on UDP, which shaped the reliable UDP milestone: connections, sequencing, acknowledgements, packet loss and round-trip time.
- [Quake 3 Source Code Review: Network Model](https://fabiensanglard.net/quake3/network.php) by Fabien Sanglard and the [Quake III Arena network channel](https://github.com/id-Software/Quake-III-Arena/blob/master/code/qcommon/net_chan.c): how a shipped game builds reliable delivery over UDP, with sequencing, delta compression and fragmentation.
- [netcode](https://github.com/mas-bandwidth/netcode): a reference design of secure client/server connections over UDP, for the connection and encryption milestones.
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/): my starting point for DNS resolution and for polling many sockets.
- [epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html), [kqueue(2)](https://man.freebsd.org/cgi/man.cgi?query=kqueue) and [WSAPoll](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll): the reference for the exact behaviour of each polling API.
- [RFC 768](https://www.rfc-editor.org/rfc/rfc768) (UDP), [RFC 9293](https://www.rfc-editor.org/rfc/rfc9293) (TCP) and [RFC 9112](https://www.rfc-editor.org/rfc/rfc9112) (HTTP/1.1): the specifications the UDP, TCP and HTTP layers follow.


## Author

[Laszlo Serdet](https://github.com/lszsrd), networking lead of VMNL.


## License

Released under the [MIT License](https://github.com/VMNL/vmnl-net/blob/main/LICENSE).
