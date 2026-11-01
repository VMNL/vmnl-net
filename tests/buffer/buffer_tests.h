/**
 * \file buffer_tests.h
 * \brief Fixtures and helpers of the buffer tests.
 */

#ifndef VMNL_NET_BUFFER_TESTS_H
#define VMNL_NET_BUFFER_TESTS_H

#include <vmnl/net/allocator.h>

#include "buffer/buffer_internal.h"

#include "utest.h"

#define NTYPES           4 /*!< Elements of #types, the count of UTEST_I. */
#define NRESIZABLE_TYPES 3 /*!< Elements of #resizable_types, the count of UTEST_I. */

/**
 * \brief Declares the fixture of a UTEST_I suite: a buffer of the type at the index of the test.
 * \param SUITE Name of the suite.
 * \param TYPES Types of the buffers, one per index.
 * \param CAPACITY Capacity of the buffers.
 * \remark The teardown restores the default allocator, then destroys the buffer.
 */
#define BUFFER_FIXTURE_I(SUITE, TYPES, CAPACITY)                                            \
    struct SUITE {                                                                          \
        VmnlNetBuffer *buffer;                                                              \
    };                                                                                      \
                                                                                            \
    UTEST_I_SETUP(SUITE)                                                                    \
    {                                                                                       \
        utest_fixture->buffer = vmnl_net_buffer_create(TYPES[utest_index], CAPACITY, NULL); \
        ASSERT_NE(utest_fixture->buffer, NULL);                                             \
    }                                                                                       \
                                                                                            \
    UTEST_I_TEARDOWN(SUITE)                                                                 \
    {                                                                                       \
        (void) utest_result;                                                                \
        (void) utest_index;                                                                 \
        vmnl_net_allocator_set(malloc, realloc, free);                                      \
        vmnl_net_buffer_destroy(utest_fixture->buffer);                                     \
    }

/**
 * \brief Declares the fixture of a UTEST_F suite: a buffer of one type.
 * \param SUITE Name of the suite.
 * \param TYPE Type of the buffer.
 * \param CAPACITY Capacity of the buffer.
 * \remark The teardown restores the default allocator, then destroys the buffer.
 */
#define BUFFER_FIXTURE_F(SUITE, TYPE, CAPACITY)                               \
    struct SUITE {                                                            \
        VmnlNetBuffer *buffer;                                                \
    };                                                                        \
                                                                              \
    UTEST_F_SETUP(SUITE)                                                      \
    {                                                                         \
        utest_fixture->buffer = vmnl_net_buffer_create(TYPE, CAPACITY, NULL); \
        ASSERT_NE(utest_fixture->buffer, NULL);                               \
    }                                                                         \
                                                                              \
    UTEST_F_TEARDOWN(SUITE)                                                   \
    {                                                                         \
        (void) utest_result;                                                  \
        vmnl_net_allocator_set(malloc, realloc, free);                        \
        vmnl_net_buffer_destroy(utest_fixture->buffer);                       \
    }

/**
 * \brief Every type, in the order of the UTEST_I indexes.
 */
static VmnlNetBufferType types[NTYPES] = {
    VMNL_NET_BUFFER_FLAT,
    VMNL_NET_BUFFER_RING,
    VMNL_NET_BUFFER_DYNAMIC,
    VMNL_NET_BUFFER_CHUNKED,
};

/**
 * \brief Types that vmnl_net_buffer_resize() and vmnl_net_buffer_shrink() support.
 */
static VmnlNetBufferType resizable_types[NRESIZABLE_TYPES] = {
    VMNL_NET_BUFFER_RING,
    VMNL_NET_BUFFER_DYNAMIC,
    VMNL_NET_BUFFER_CHUNKED,
};

/**
 * \brief Allocations left before counting_allocate() and counting_reallocate() return NULL.
 */
static size_t allowed_allocations;

/**
 * \brief Blocks allocated and not freed since allocator_limit().
 */
static size_t live_allocations;

/**
 * \brief Calls to counting_deallocate() since allocator_limit().
 */
static size_t deallocations;

/**
 * \brief malloc() limited by #allowed_allocations.
 * \param size Bytes to allocate.
 * \return Allocated block, NULL once #allowed_allocations is 0.
 */
static inline void *counting_allocate(size_t size)
{
    void *pointer = (allowed_allocations > 0) ? malloc(size) : NULL;

    if (pointer != NULL) {
        allowed_allocations--;
        live_allocations++;
    }
    return pointer;
}

/**
 * \brief realloc() limited by #allowed_allocations.
 * \param pointer Block to reallocate, nullable.
 * \param size Bytes to allocate.
 * \return Reallocated block, NULL once #allowed_allocations is 0.
 */
static inline void *counting_reallocate(void *pointer, size_t size)
{
    void *reallocated = (allowed_allocations > 0) ? realloc(pointer, size) : NULL;

    if (reallocated != NULL) {
        allowed_allocations--;
        live_allocations += (pointer == NULL) ? 1 : 0;
    }
    return reallocated;
}

/**
 * \brief free() counted in #live_allocations and #deallocations.
 * \param pointer Block to free, nullable.
 */
static inline void counting_deallocate(void *pointer)
{
    live_allocations -= (pointer != NULL) ? 1 : 0;
    deallocations++;
    free(pointer);
}

/**
 * \brief Sets the counting allocator, and resets its counters.
 * \param nallocations Allocations allowed before the allocator fails.
 */
static inline void allocator_limit(size_t nallocations)
{
    allowed_allocations = nallocations;
    live_allocations = 0;
    deallocations = 0;
    vmnl_net_allocator_set(counting_allocate, counting_reallocate, counting_deallocate);
}

/**
 * \brief Sums the sizes of views.
 * \param views Views to sum.
 * \param nviews Number of elements of \p views.
 * \return Bytes described by \p views.
 */
static inline size_t views_size(const VmnlNetBufferView *views, size_t nviews)
{
    size_t size = 0;

    for (size_t it = 0; it < nviews; it++) {
        size += views[it].size;
    }
    return size;
}

/**
 * \brief Prepares, writes and commits the characters of a string.
 * \param buffer Buffer to write to.
 * \param bytes String to write, without its null character.
 * \return true on success, false otherwise.
 */
static inline bool append(VmnlNetBuffer *buffer, const char *bytes)
{
    size_t nbytes = strlen(bytes);
    VmnlNetBufferView views[8];
    size_t nviews = 8;
    size_t written = 0;
    size_t size;

    if (vmnl_net_buffer_prepare(buffer, nbytes, views, &nviews, NULL) == false) {
        return false;
    }
    for (size_t it = 0; it < nviews && written < nbytes; it++) {
        size = (views[it].size < nbytes - written) ? views[it].size : nbytes - written;
        memcpy(views[it].data, bytes + written, size);
        written += size;
    }
    return vmnl_net_buffer_commit(buffer, nbytes, NULL);
}

/**
 * \brief Prepares, writes and commits bytes 'x'.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes to write.
 * \return true on success, false otherwise.
 */
static inline bool commit(VmnlNetBuffer *buffer, size_t nbytes)
{
    char *bytes = malloc(nbytes + 1);
    bool written;

    memset(bytes, 'x', nbytes);
    bytes[nbytes] = '\0';
    written = append(buffer, bytes);
    free(bytes);
    return written;
}

/**
 * \brief Checks the stored bytes of a buffer.
 * \param buffer Buffer to read.
 * \param bytes Expected stored bytes, as a string.
 * \return true if the stored bytes are the characters of \p bytes, false otherwise.
 */
static inline bool data_equals(const VmnlNetBuffer *buffer, const char *bytes)
{
    VmnlNetBufferView views[8];
    size_t nviews = 8;
    size_t read = 0;

    if (vmnl_net_buffer_size(buffer) != strlen(bytes) || vmnl_net_buffer_data(buffer, 0, views, &nviews) != strlen(bytes)) {
        return false;
    }
    for (size_t it = 0; it < nviews; it++) {
        if (memcmp(views[it].data, bytes + read, views[it].size) != 0) {
            return false;
        }
        read += views[it].size;
    }
    return true;
}

/**
 * \brief Gets the address of the first stored byte of a buffer.
 * \param buffer Buffer to read.
 * \return Address of the first stored byte, NULL if \p buffer stores none.
 */
static inline const uint8_t *data_start(const VmnlNetBuffer *buffer)
{
    VmnlNetBufferView view = {0};

    vmnl_net_buffer_data(buffer, 0, &view, &(size_t) {1});
    return view.data;
}

#endif /* !VMNL_NET_BUFFER_TESTS_H */
