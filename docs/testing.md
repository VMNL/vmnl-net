# Testing

## Tests

Tests use [utest.h](https://github.com/sheredom/utest.h), fetched by CMake, and follow the formatting of the library.

- one file per public function, named like its source, for the behavior every type shares: `tests/buffer/buffer_prepare_tests.c`
- one file per type and public function, in a folder per type, for the behavior of that type only: `tests/buffer/ring/buffer_ring_prepare_tests.c`
- the files listed in `tests/CMakeLists.txt`, and the helpers of a module in `tests/<module>/<module>_tests.h`
- the C library comes from `utest.h`, which the tests do not include again
- the generic containers, like the list, are tested directly through their internal header, one file per function or macro: `tests/list/list_splice_tests.c`; the other internal functions are covered through the public ones
- the suite is the name of the function or the macro, with the type for the behavior of one type: `vmnl_net_buffer_prepare`, `vmnl_net_buffer_ring_prepare`, `VMNL_NET_LIST_FOREACH`
- the behavior every type shares runs once per type with `UTEST_I`, its index following the `types` of `tests/buffer/buffer_tests.h`: `vmnl_net_buffer_prepare.keeps_stored_bytes/1` is the ring buffer
- each test says what the function does, `<verb>_<object>_<condition>`, in snake case, the verb first, without article, `vmnl_net` nor the type of the suite, the condition optional and starting with `when`, `with`, `without` or `after`: `returns_null_when_empty`, `prepares_two_views_when_free_space_wraps`
- the recurring cases keep the same name:

| Case | Name |
| --- | --- |
| Value returned | `returns_<value>`: `returns_zero_when_empty` |
| Property kept | `keeps_<property>`: `keeps_stored_bytes` |
| Documented no-op | `ignores_<argument>`: `ignores_zero_bytes` |
| `VMNL_NET_EINVAL` | `rejects_<argument>`: `rejects_zero_capacity` |
| `VMNL_NET_ENOTSUP` | `fails_as_unsupported` |
| `VMNL_NET_ENOBUFS` | `fails_when_full`, `fails_with_more_than_free_space` |
| `VMNL_NET_ENOMEM` | `fails_without_memory`, `fails_without_memory_for_<allocation>` |
| NULL error | `accepts_null_error` |

- the tests go from the nominal case to the properties, the limits, the errors and the nullable parameters, the NULL error last
- the tests check the documented contract, not the implementation: a behavior without documentation is documented before it is tested
- every documented limit is tested exactly at the limit and one past it: a capacity, the free space, the end of a chunk or of a block, an offset equal to the size
- one behavior per test, with `ASSERT` for the steps that set the case up and `EXPECT` for what the test checks
- the tests are independent: each one frees what it creates, its fixture resets the global state, and the `sanitize` preset runs them under AddressSanitizer and UndefinedBehaviorSanitizer
- a `VmnlNetError` starts at `UINT32_MAX`, so the test proves the function writes it
- a test that replaces the allocator restores the default one before its first `ASSERT`, or lets the teardown of its fixture do it
- the teardown runs even after a failed `ASSERT`, which matters because every test runs in the same process

```c
BUFFER_FIXTURE_I(vmnl_net_buffer_consume, types, 16)

UTEST_I(vmnl_net_buffer_consume, drops_first_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "llo"));
}
```

## Benchmarks

Benchmarks use [Google Benchmark](https://github.com/google/benchmark), fetched by CMake when `VMNL_NET_BUILD_BENCHMARKS` is `ON`, and they are the only C++ code of the repository.
Each public function has its own benchmark file, `bench/<module>/<name>_benchmarks.cpp`, listed in `bench/CMakeLists.txt`.
A benchmark takes the name of the function without the `vmnl_net_` prefix, which would clash with the function itself, followed by the case when there are several, like `buffer_prepare_ring`, and covers its sizes with `Range` or `Args` rather than one benchmark per size.
To measure a change, compare the runs before and after it with the `compare.py` tool of Google Benchmark.
