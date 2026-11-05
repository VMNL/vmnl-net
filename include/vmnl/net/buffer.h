/**
 * \file buffer.h
 *
 * \defgroup Buffer Buffer
 * \addtogroup Buffer
 * \brief Byte queues written and read in place.
 *
 * A buffer is a queue of bytes, read in the order they were written.
 * Its memory holds the stored bytes, written and not consumed yet, then the free space for the next ones.
 *
 * The buffer functions never copy bytes themselves. They return views, a pointer and a size in the memory of the buffer, where the application writes and reads directly, so recv() and send() need no intermediate copy, see VmnlNetBufferView.
 *
 * \par Types
 * The four types answer different memory needs behind the same functions, as the views describe their memory whatever its layout.
 *
 * |                      | Flat                          | Ring                    | Dynamic                         | Chunked                      |
 * |----------------------|-------------------------------|-------------------------|---------------------------------|------------------------------|
 * | When full            | Fails, fixed memory           | Fails, fixed memory     | Grows, copies its bytes         | Adds a chunk, no copy        |
 * | Views returned       | 1                             | 1, or 2 when wrapped    | 1                               | 1 per chunk                  |
 * | Reading at an offset | Constant                      | Constant                | Constant                        | Walks the chunks before it   |
 * | Bytes moved on write | To the front, when needed     | Never                   | To the front, or to a new block | Never                        |
 * | Resizable            | No                            | Yes                     | Yes                             | Yes                          |
 * | Best use case        | Packets of known maximum size | Streams in fixed memory | Messages of unknown size        | Large transfers, like files  |
 *
 * Dynamic suits most uses, while flat and ring keep a fixed memory and chunked grows without copying.
 *
 * \par Creating a buffer
 * Buffers are allocated with the global allocator, see vmnl_net_allocator_set(), and their type is picked from the table of the types above.
 *
 * \snippet example_buffer_basics.c create_buffer
 *
 * \par Writing to a view
 * 1. vmnl_net_buffer_prepare() returns views on at least the free bytes asked for.
 * 2. The bytes are written into these views, with memcpy() or recv() for example.
 * 3. vmnl_net_buffer_commit() turns the bytes written into stored bytes.
 *
 * \snippet example_buffer_basics.c write_buffer
 *
 * \par Reading from a view
 * 1. vmnl_net_buffer_data() returns views on the stored bytes, and keeps them stored.
 * 2. The bytes are read from these views, with memcpy() or send() for example, as many times as needed.
 * 3. vmnl_net_buffer_consume() drops the bytes that were handled, which frees their space.
 *
 * \snippet example_buffer_basics.c read_buffer
 *
 * \par Destroying a buffer
 * Destroying a buffer frees its memory, and invalidates its views.
 *
 * \snippet example_buffer_basics.c destroy_buffer
 *
 * \par Holding a secret
 * No function erases bytes, as consuming, clearing and destroying only move offsets or free memory.
 * Erase the bytes of a secret through the views of vmnl_net_buffer_data() before dropping them, with a call the compiler cannot remove, like explicit_bzero(), memset_s() or SecureZeroMemory().
 * Keep it in a chunked buffer, or in a ring buffer never resized. Flat and dynamic buffers move their stored bytes, and resizing leaves a copy of them in the memory it frees, out of reach of the views.
 * @{
 */

#ifndef VMNL_NET_BUFFER_H
#define VMNL_NET_BUFFER_H

#include <vmnl/net/attribute.h>
#include <vmnl/net/error.h>

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque buffer.
 * \attention A buffer **MUST** be used by one thread at a time, as it has no lock.
 */
typedef struct VmnlNetBuffer VmnlNetBuffer;

/**
 * \brief Contiguous bytes in the memory of a buffer.
 * \attention
 * - The bytes of a view **MUST** not be freed, as the buffer owns them and frees them with vmnl_net_buffer_destroy().
 * - A view **MUST** not be used after a call that modifies its buffer, as the bytes may have moved, or been dropped.
 * \remark The views returned by the buffer functions are never empty.
 */
typedef struct VmnlNetBufferView {
    uint8_t *data; /*!< First byte. */
    size_t size; /*!< Number of bytes. */
} VmnlNetBufferView;

/**
 * \brief Type of a buffer, see the types of the \ref Buffer module.
 */
typedef uint32_t VmnlNetBufferType;

#define VMNL_NET_BUFFER_FLAT    UINT32_C(1) /*!< Fixed capacity, one view. */
#define VMNL_NET_BUFFER_RING    UINT32_C(2) /*!< Fixed capacity, wraps around, up to two views. */
#define VMNL_NET_BUFFER_DYNAMIC UINT32_C(3) /*!< Grows when full, one view. */
#define VMNL_NET_BUFFER_CHUNKED UINT32_C(4) /*!< Grows by chunks, one view per chunk. */

/**
 * \brief Creates a buffer.
 * \param type Type of the buffer, one of #VMNL_NET_BUFFER_FLAT, #VMNL_NET_BUFFER_RING, #VMNL_NET_BUFFER_DYNAMIC or #VMNL_NET_BUFFER_CHUNKED.
 * \param capacity Bytes allocated up front, rounded up to whole chunks for a chunked buffer, 0 to allocate on the first write of a dynamic or chunked buffer.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p type is unknown, or \p capacity is 0 for a flat or ring buffer.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 */
VMNL_NET_API VMNL_NET_NODISCARD VmnlNetBuffer *vmnl_net_buffer_create(VmnlNetBufferType type, size_t capacity, VmnlNetError *error);

/**
 * \brief Counts the stored bytes of a buffer.
 * \param buffer Buffer to check.
 * \return Number of stored bytes, at most vmnl_net_buffer_capacity().
 */
VMNL_NET_API size_t vmnl_net_buffer_size(const VmnlNetBuffer *buffer);

/**
 * \brief Counts the allocated bytes of a buffer.
 * \param buffer Buffer to check.
 * \return Number of allocated bytes, stored and free.
 */
VMNL_NET_API size_t vmnl_net_buffer_capacity(const VmnlNetBuffer *buffer);

/**
 * \brief Checks whether a buffer is empty.
 * \param buffer Buffer to check.
 * \return true if \p buffer stores no byte, false otherwise.
 */
VMNL_NET_API bool vmnl_net_buffer_is_empty(const VmnlNetBuffer *buffer);

/**
 * \brief Drops every stored byte of a buffer, and keeps its capacity.
 * \param buffer Buffer to clear.
 */
VMNL_NET_API void vmnl_net_buffer_clear(VmnlNetBuffer *buffer);

/**
 * \brief Grows the capacity of a buffer.
 * \param buffer Buffer to grow.
 * \param capacity Capacity to grow to, rounded up to whole chunks for a chunked buffer.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOTSUP: \p buffer is flat.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, including when \p buffer already has \p capacity bytes or more, false otherwise.
 */
VMNL_NET_API bool vmnl_net_buffer_resize(VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error);

/**
 * \brief Reduces the capacity of a buffer.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to, 0 to free all the memory of a dynamic or chunked buffer until its next write.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p capacity is less than vmnl_net_buffer_size(), or 0 for a ring buffer.
 * - #VMNL_NET_ENOTSUP: \p buffer is flat.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, including when \p buffer already has \p capacity bytes or less, false otherwise.
 * \remark A chunked buffer frees only its free chunks, from the end, so its capacity stays a whole number of chunks, above \p capacity when its stored bytes span more chunks.
 */
VMNL_NET_API bool vmnl_net_buffer_shrink(VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error);

/**
 * \brief Describes free space of a buffer to write into.
 * \param buffer Buffer to write to.
 * \param nbytes Free bytes needed, at least, 0 for no view.
 * \param views Free space in order, at least \p nbytes and as much as the buffer has.
 * \param nviews Number of elements of \p views on input, number of views filled on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p views is too small to describe \p nbytes.
 * - #VMNL_NET_ENOBUFS: \p buffer is flat or ring, and lacks \p nbytes of free space.
 * - #VMNL_NET_ENOMEM: memory allocation failure, or \p nbytes too large to ever fit.
 * \return true on success, false otherwise.
 * \attention The return value **MUST** be checked, as \p views describes nothing after a failure.
 */
VMNL_NET_API VMNL_NET_NODISCARD bool vmnl_net_buffer_prepare(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Appends the first prepared bytes to the stored bytes of a buffer.
 * \param buffer Buffer written to.
 * \param nbytes Bytes written at the start of the views of the last vmnl_net_buffer_prepare(), in order, 0 for none.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than the free space after the stored bytes.
 * \return true on success, false otherwise.
 * \attention The bytes **MUST** be written in the views of the last vmnl_net_buffer_prepare(), as nothing is copied and they are only counted as stored.
 */
VMNL_NET_API bool vmnl_net_buffer_commit(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetError *error);

/**
 * \brief Describes the stored bytes of a buffer to read.
 * \param buffer Buffer to read.
 * \param offset Stored bytes to skip from the first one.
 * \param views Stored bytes from \p offset, in order.
 * \param nviews Number of elements of \p views on input, number of views filled on output.
 * \return Number of bytes described by \p views, 0 if \p offset is at least vmnl_net_buffer_size().
 * \remark Too few \p views describe only the first bytes, calling again past them gets the rest:
 * \code
 * VmnlNetBufferView views[4];
 * size_t nviews = 4;
 * size_t nbytes;
 *
 * // skips the bytes in flight, 4 views at a time, until none is left
 * while ((nbytes = vmnl_net_buffer_data(buffer, in_flight, views, &nviews)) > 0) {
 *     for (size_t it = 0; it < nviews; it++) {
 *         send_packet(peer, views[it].data, views[it].size);
 *     }
 *     in_flight += nbytes;
 *     nviews = 4;
 * }
 * \endcode
 */
VMNL_NET_API size_t vmnl_net_buffer_data(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews);

/**
 * \brief Drops the first stored bytes of a buffer.
 * \param buffer Buffer read from.
 * \param nbytes Bytes to drop, once sent or read.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than vmnl_net_buffer_size().
 * \return true on success, false otherwise.
 * \remark A chunked buffer keeps the chunks it empties for the next writes.
 */
VMNL_NET_API bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetError *error);

/**
 * \brief Destroys a buffer.
 * \param buffer Buffer to destroy, nullable.
 */
VMNL_NET_API void vmnl_net_buffer_destroy(VmnlNetBuffer *buffer);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_BUFFER_H */

/** @} */
