# Conventions

clang-format takes care of the layout of the sources, the tests and the benchmarks, the rules below cover the rest.

## Names

They follow the [rules of VMNL](https://github.com/VMNL/vmnl/blob/main/docs/INSTRUCTIONS.md).

| Element | Form | Example |
| --- | --- | --- |
| Type | `VmnlNet<Name>` | `VmnlNetBuffer` |
| Function | `vmnl_net_<module>_<action>` | `vmnl_net_buffer_create` |
| Static function | `vmnl_net_<module>_<type>_<action>`, like the others, so that two files never share a name | `vmnl_net_buffer_ring_prepare_after_wrap` |
| Macro | `VMNL_NET_<NAME>` | `VMNL_NET_EINVAL` |
| Predicate | `vmnl_net_<module>_is_<state>` | `vmnl_net_buffer_is_empty` |
| Conversion | `vmnl_net_<module>_as_<type>` | `vmnl_net_error_as_string` |

## Files

- one public function per file, named without the `vmnl_net_` prefix: `src/buffer/buffer_create.c`
- the operations behind a public function in one folder per type or family: an operation shared by a whole family takes its name, like `src/buffer/contiguous/buffer_contiguous_clear.c`, and one shared by some types keeps the name of the type that defines it, the others pointing to it or calling it, like the dynamic buffer committing with `vmnl_net_buffer_flat_commit()`
- private code in `src/<module>/<module>_internal.h`
- an internal module used by several others in its own folder, like `src/list/`
- platform code in `posix/` or `win32/`
- includes in four blocks: public headers of the library, private ones, dependencies, C library
- a file includes only what it needs, and not a header that another of its includes already brings

## Errors and NULL

- a function that can fail takes a nullable `VmnlNetError *error` last, sets it on every call, and tells the failure through its return value
- NULL is accepted only where it means something: an `error` the caller does not read, a `data` it does not need, or nothing to destroy
- any other pointer is used without check, like `strlen(NULL)` in the libc, so a NULL one is a bug of the caller

## Not checked by clang-format

- `sizeof` without parentheses on an expression: `sizeof *context`
- counters with `it++` and `it--`, not `++it` and `--it`, unless the expression needs the updated value
- comparisons written out for readability: booleans with `== true` or `== false`, pointers with `== NULL` or `!= NULL`, not `!value` nor `value` alone
- the condition of a ternary in parentheses: `(now > start) ? now - start : 0`
- `const` only on what a pointer points to and on static tables, not on values, so a parameter can count down instead of a copy: `size_t nbytes`, `const VmnlNetBuffer *buffer`
- no commented-out code, since Git keeps the old versions
- a comment above the line it explains, and at its right only for the short state or result the line produces, like `// [alice, bob]`
- a named constant rather than a magic number: `VMNL_NET_BUFFER_CHUNK_SIZE`, not `16384`
- predicates stated in the positive: `vmnl_net_buffer_is_empty`, not `vmnl_net_buffer_is_not_empty`
- a value of an argument that makes a function behave differently gets a named constant, unless the plain value reads naturally, like an `interval` of `0` for a one-shot timer

## Doxygen

- short and technical, with snippets that compile, and each fact said once on a page
- in the public headers, the operation as the caller sees it: what it does, its parameters, its errors and its conditions of use, the behavior it can rely on rather than the system call behind it, without implementation detail, number copied from an internal constant or plan for later
- in the internal headers, the contract of the implementation: what the public function already checked, what the operation must do and keep true, the cases it must handle, and the details the next developer needs
- a `\brief` that says what the module or the function is for, the details in the `\param` and `\return` they belong to, and a `\remark` only for an important fact that fits nowhere else
- `nullable` for optional pointers, and the error codes listed under the `error` parameter
- the contracts in `\attention`, with `**MUST**` and a short reason, several contracts of a function in one `\attention` list
- `\iso{function,path}` links a function of the C standard to its cppreference page, like `\iso{malloc,memory}`, and `\posix{function,section}` a POSIX or Linux function to its man7 page, like `\posix{clock_gettime,2}`
- these links go in the descriptions of the modules only, as an IDE shows the documentation of a function or a type in a popup where a link only adds noise

## Markdown

Markdown files are written in English, one sentence per line, with a single blank line before each heading.
