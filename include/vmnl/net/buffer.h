/**
 * \file buffer.h
 *
 * \defgroup Buffer Buffer
 * \addtogroup Buffer
 * \brief Byte buffers of the library.
 *
 * A buffer is a queue of bytes accessed through views of its own memory, without copy: vmnl_net_buffer_prepare() and vmnl_net_buffer_commit() write into its free space, vmnl_net_buffer_data() and vmnl_net_buffer_consume() read its stored bytes in the same order.
 * Buffers are allocated with the global allocator, see vmnl_net_allocator_set().
 *
 * \par Types
 * | Type                     | When full   | Views       | Capacity 0               | Use                           |
 * | ------------------------ | ----------- | ----------- | ------------------------ | ----------------------------- |
 * | #VMNL_NET_BUFFER_FLAT    | Fails       | 1           | Rejected                 | Packets of known maximum size |
 * | #VMNL_NET_BUFFER_DYNAMIC | Grows       | 1           | Allocates on first write | Messages of unknown size      |
 * | #VMNL_NET_BUFFER_RING    | Fails       | Up to 2     | Rejected                 | Streams in bounded memory     |
 * | #VMNL_NET_BUFFER_CHUNKS  | Adds chunks | 1 per chunk | Allocates on first write | Large streams                 |
 *
 * Every type but flat can be resized with vmnl_net_buffer_resize() and vmnl_net_buffer_shrink().
 *
 * \par Example
 * \code
 * VmnlNetError error;
 * VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_DYNAMIC, 0, &error);
 * VmnlNetBufferView view;
 * size_t nviews = 1;
 *
 * if (buffer == NULL) {
 *     printf("%s\n", vmnl_net_error_as_string(error)); // Cannot allocate memory
 *     return 1;
 * }
 * vmnl_net_buffer_prepare(buffer, 5, &view, &nviews, NULL); // view: free space
 * memcpy(view.data, "hello", 5);
 * vmnl_net_buffer_commit(buffer, 5, NULL);                  // "hello"
 * vmnl_net_buffer_data(buffer, 0, &view, &nviews);          // view: "hello"
 * fwrite(view.data, 1, view.size, stdout);
 * vmnl_net_buffer_consume(buffer, 5, NULL);                 // ""
 * vmnl_net_buffer_destroy(buffer);
 * \endcode
 * @{
 */

#ifndef VMNL_NET_BUFFER_H
#define VMNL_NET_BUFFER_H

#include <vmnl/net/attribute.h>
#include <vmnl/net/error.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque buffer.
 * \attention A buffer **MUST** be used by one thread at a time: it has no lock.
 */
typedef struct VmnlNetBuffer VmnlNetBuffer;

/**
 * \brief Bytes of a buffer, described in place.
 * \attention A view **MUST** not be used after a call that modifies its buffer: the bytes may have moved, or been dropped.
 */
typedef struct VmnlNetBufferView {
    uint8_t *data; /*!< Pointer to a contiguous memory region. */
    size_t size; /*!< Number of bytes. */
} VmnlNetBufferView;

/**
 * \brief Type of a buffer, see the types of the \ref Buffer module.
 * \remark The types start at 1: a zeroed value is rejected instead of being taken for a type.
 */
typedef uint32_t VmnlNetBufferType;

#define VMNL_NET_BUFFER_FLAT    UINT32_C(1) /*!< Fixed capacity, one view. */
#define VMNL_NET_BUFFER_DYNAMIC UINT32_C(2) /*!< Grows when full, one view. */
#define VMNL_NET_BUFFER_RING    UINT32_C(3) /*!< Fixed capacity, wraps around, up to two views. */
#define VMNL_NET_BUFFER_CHUNKS  UINT32_C(4) /*!< Grows by chunks, one view per chunk. */

/**
 * \brief Creates a buffer.
 * \param type Type of the buffer, one of the VMNL_NET_BUFFER_* constants.
 * \param capacity Bytes allocated up front, 0 to allocate on the first write of a dynamic or chunk buffer.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p type is unknown, or \p capacity is 0 for a flat or ring buffer.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 * \remark A flat buffer never changes its capacity.
 * \remark A dynamic buffer grows to 1.5 times its capacity, or to the bytes needed if more, and to 256 bytes at least, when vmnl_net_buffer_prepare() lacks room.
 * \remark A ring buffer wraps its stored bytes around the end of its memory instead of moving them to the front.
 * \remark A chunk buffer rounds \p capacity up to whole chunks, each one allocation, so growing adds chunks without copying the stored bytes.
 */
VMNL_NET_API VMNL_NET_NODISCARD VmnlNetBuffer *vmnl_net_buffer_create(const VmnlNetBufferType type, const size_t capacity, VmnlNetError *error);

/**
 * \brief Gets the stored bytes of a buffer.
 * \param buffer Buffer to check.
 * \return Number of stored bytes, at most vmnl_net_buffer_capacity().
 */
VMNL_NET_API size_t vmnl_net_buffer_size(const VmnlNetBuffer *buffer);

/**
 * \brief Gets the allocated bytes of a buffer.
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
 * \brief Drops every stored byte of a buffer.
 * \param buffer Buffer to clear.
 * \remark Keeps the capacity and does not erase the bytes: only the positions are reset, in O(1), or in O(n) in the chunks of a chunk buffer.
 */
VMNL_NET_API void vmnl_net_buffer_clear(VmnlNetBuffer *buffer);

/**
 * \brief Resizes the capacity of a buffer.
 * \param buffer Buffer to grow.
 * \param capacity Capacity to grow to.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOTSUP: \p buffer is flat.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Nothing is done, successfully, if the capacity is already \p capacity or more.
 * \remark A ring buffer copies its stored bytes to the front of its new memory, unwrapped.
 * \remark A chunk buffer rounds \p capacity up to whole chunks.
 */
VMNL_NET_API bool vmnl_net_buffer_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Reduces the capacity of a buffer.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p capacity is less than vmnl_net_buffer_size().
 * - #VMNL_NET_ENOTSUP: \p buffer is flat.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Nothing is done, successfully, if the capacity is already \p capacity or less.
 * \remark A ring buffer copies its stored bytes to the front of its new memory, unwrapped.
 * \remark A chunk buffer frees only its free chunks, from the end: its capacity is rounded up to whole chunks, and can stay above \p capacity while bytes are stored past it.
 */
VMNL_NET_API bool vmnl_net_buffer_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Gets the free space of a buffer to write into, without copy.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, in order.
 * \param nviews Size of \p views on input, views filled on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p views is too small to describe \p nbytes.
 * - #VMNL_NET_ENOBUFS: \p flat buffer or ring buffer lacks room.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark The views describe at least \p nbytes, and as much free space as they can, so one recv() can receive more.
 * \remark A flat or dynamic buffer first moves its stored bytes to the front of its memory, when the bytes consumed before them make enough room.
 * \remark Nothing is stored until vmnl_net_buffer_commit().
 */
VMNL_NET_API bool vmnl_net_buffer_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Stores the first prepared bytes of a buffer.
 * \param buffer Buffer written to.
 * \param nbytes Bytes written at the start of the views of vmnl_net_buffer_prepare().
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than the free space.
 * \return true on success, false otherwise.
 * \attention The bytes **MUST** be written in the views of the last vmnl_net_buffer_prepare(): the end of the stored bytes moves over them, without copy.
 */
VMNL_NET_API bool vmnl_net_buffer_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error);

/**
 * \brief Drops the first stored bytes of a buffer.
 * \param buffer Buffer read from.
 * \param nbytes Bytes to drop, once sent or read.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than vmnl_net_buffer_size().
 * \return true on success, false otherwise.
 * \remark Moves the start of the stored bytes without erasing them, and a chunk buffer moves the chunks it empties to the end of its list, to reuse them.
 */
VMNL_NET_API bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error);

/**
 * \brief Gets the stored bytes of a buffer to read, without copy.
 * \param buffer Buffer to read.
 * \param offset Stored bytes to skip.
 * \param views Stored bytes from \p offset, in order.
 * \param nviews Size of \p views on input, views filled on output.
 * \return Number of bytes described by \p views, 0 if \p offset is at least vmnl_net_buffer_size().
 * \remark Nothing is dropped until vmnl_net_buffer_consume().
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
VMNL_NET_API size_t vmnl_net_buffer_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews);

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
