# Testing

## Tests

Tests use [utest.h](https://github.com/sheredom/utest.h), fetched by CMake.

### Files

- one file per public function, named like its source: `tests/buffer/buffer_prepare_tests.c`
- the behavior of a single buffer type in a folder of its own: `tests/buffer/ring/buffer_ring_prepare_tests.c`
- the helpers of a module in `tests/<module>/<module>_tests.h`, and every file listed in `tests/CMakeLists.txt`
- the containers, like the list, tested through their internal header, the other internal functions through the public ones
- no C library include, as `utest.h` brings it

### Names

- the suite is the name of the function or the macro, with the type for the behavior of one type: `vmnl_net_buffer_ring_prepare`
- the test says what the function does, as `<verb>_<object>_<condition>`, without article nor the words of the suite, the condition starting with `when`, `with`, `without` or `after`: `prepares_two_views_when_free_space_wraps`
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

### Writing a test

- one behavior per test, with `ASSERT` to set the case up and `EXPECT` to check it
- a test checks the documented contract, not the implementation, so a behavior is documented before it is tested
- each limit tested at its value and one past it
- from the nominal case to the limits, the errors and the nullable parameters, the NULL error last
- a `VmnlNetError` set to `UINT32_MAX` first, to prove the function writes it
- each test frees what it creates, and the `sanitize` preset checks it
- a test that replaces the allocator restores it before its first `ASSERT`, or leaves it to its fixture, whose teardown runs even after a failure
- a behavior shared by the types of a module is written once with `UTEST_I`, which runs it once per type, like the buffer types of `tests/buffer/buffer_tests.h`

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

Benchmarks use [Google Benchmark](https://github.com/google/benchmark), fetched by CMake when `VMNL_NET_BUILD_BENCHMARKS` is `ON`.
Each public function has its file, `bench/<module>/<name>_benchmarks.cpp`, listed in `bench/CMakeLists.txt`.
A benchmark is named after its function without `vmnl_net_`, plus its case when there are several, like `buffer_prepare_ring`, and covers its sizes with `Range` or `Args`.
Compare the runs before and after a change with the `compare.py` tool of Google Benchmark.
