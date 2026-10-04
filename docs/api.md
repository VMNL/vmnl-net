# API

This page lists every public function of `vmnl_net`, grouped by module.
Include `<vmnl/net.h>` to get the whole API, or the header of a single module.


## Core

Declared in `<vmnl/net/core.h>`.

| Function | Description |
| --- | --- |
| vmnl_net_error_string() | Gets a description of an error code. |
| vmnl_net_version() | Gets the version of the library as an unsigned 32-bit integer. |
| vmnl_net_version_string() | Gets the version of the library as a string. |
| vmnl_net_set_allocator() | Sets the functions used by the library to allocate memory. |


## Context

Declared in `<vmnl/net/context.h>`.

| Function | Description |
| --- | --- |
| vmnl_net_context_create() | Creates a context. |
| vmnl_net_context_destroy() | Destroys a context. |

