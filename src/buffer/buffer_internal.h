/**
 * \file buffer_internal.h
 * \brief Internal definition of the buffers.
 *
 * Every type of buffer starts with a VmnlNetBuffer that points to its VmnlNetBufferOps: a public function checks its arguments, then calls the operation of the type.
 * An operation that cannot fail once the arguments are checked returns nothing, and its public function writes the error code.
 * The operations fit together like blocks: one shared by a whole family takes its name, like vmnl_net_buffer_contiguous_data() for the flat, dynamic and ring buffers, and one shared by some types keeps the name of the type that defines it, the others pointing to it or calling it, like the dynamic buffer committing with vmnl_net_buffer_flat_commit().
 *
 * \par Example
 * \code
 * bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
 * {
 *     if (nbytes > buffer->size) {
 *         vmnl_net_error_set(error, VMNL_NET_EINVAL);
 *         return false;
 *     }
 *     buffer->ops->consume(buffer, nbytes);
 *     vmnl_net_error_set(error, VMNL_NET_SUCCESS);
 *     return true;
 * }
 * \endcode
 */

#ifndef VMNL_NET_BUFFER_INTERNAL_H
#define VMNL_NET_BUFFER_INTERNAL_H

#include <vmnl/net/buffer.h> /* IWYU pragma: export */

#include "list/list_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Bytes of every chunk of a chunk buffer, until the chunks come from a pool of the context.
 */
#define VMNL_NET_BUFFER_CHUNK_SIZE ((size_t) 16384)

/**
 * \brief Operations of one type of buffer.
 * \attention An operation **MUST** be called once its public function checked the arguments: it does not check them again.
 */
struct VmnlNetBufferOps {
    VmnlNetBuffer *(*create)(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error); /*!< Implements vmnl_net_buffer_create(), ops being this very table. */
    void (*clear)(VmnlNetBuffer *buffer); /*!< Implements vmnl_net_buffer_clear(). */
    bool (*resize)(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error); /*!< Implements vmnl_net_buffer_resize(), NULL if the type cannot grow. */
    bool (*shrink)(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error); /*!< Implements vmnl_net_buffer_shrink(), NULL if the type cannot shrink. */
    size_t (*data)(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews); /*!< Implements vmnl_net_buffer_data(). */
    void (*consume)(VmnlNetBuffer *buffer, const size_t nbytes); /*!< Implements vmnl_net_buffer_consume(). */
    bool (*prepare)(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error); /*!< Implements vmnl_net_buffer_prepare(). */
    bool (*commit)(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error); /*!< Implements vmnl_net_buffer_commit(). */
    void (*destroy)(VmnlNetBuffer *buffer); /*!< Implements vmnl_net_buffer_destroy(). */
};

/**
 * \brief Common part of every type of buffer, opaque outside the library.
 */
struct VmnlNetBuffer {
    const struct VmnlNetBufferOps *ops; /*!< Operations of the type. */
    size_t capacity; /*!< Allocated bytes. */
    size_t size; /*!< Stored bytes, at most capacity. */
};

/**
 * \brief Flat, dynamic or ring buffer, its bytes in one block.
 * \remark The stored bytes run from offset, and in a ring buffer continue at the start of the block once they reach its end.
 */
struct VmnlNetBufferContiguous {
    struct VmnlNetBuffer base; /*!< Common part, first so that a VmnlNetBuffer pointer converts to it. */
    size_t offset; /*!< Index of the first stored byte. */
    uint8_t *pointer; /*!< Allocated bytes, a block apart from the buffer: reallocating it keeps the address of the buffer, held by the caller. */
};

/**
 * \brief Chunk buffer, its bytes in a list of chunks.
 */
struct VmnlNetBufferChunked {
    struct VmnlNetBuffer base; /*!< Common part, first so that a VmnlNetBuffer pointer converts to it. */
    struct VmnlNetListLink chunks; /*!< Head of the chunks, from the first stored byte to the free space. */
    size_t chunk_size; /*!< Bytes of every chunk. */
};

/**
 * \brief Chunk of a chunk buffer, allocated in one block with its bytes right after it.
 * \remark Allocated with sizeof(struct VmnlNetBufferChunk) + chunk_size bytes, and freed in one call: its bytes are not a separate allocation.
 */
struct VmnlNetBufferChunk {
    struct VmnlNetListLink link; /*!< Link in the chunks of its buffer. */
    size_t offset; /*!< Index of the first stored byte. */
    size_t size; /*!< Stored bytes of the chunk. */
    uint8_t bytes[]; /*!< Bytes of the chunk, allocated with it. */
};

/**
 * \brief Creates a contiguous buffer.
 * \param ops Operations of the type, stored in the buffer.
 * \param capacity Bytes allocated, 0 to allocate on the first write.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 * \remark Creates the dynamic buffer, and the flat and ring buffers once vmnl_net_buffer_flat_create() rejected a capacity of 0.
 */
VmnlNetBuffer *vmnl_net_buffer_contiguous_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error);

/**
 * \brief Drops every stored byte of a contiguous buffer, in O(1).
 * \param buffer Buffer to clear.
 */
void vmnl_net_buffer_contiguous_clear(VmnlNetBuffer *buffer);

/**
 * \brief Gets the stored bytes of a contiguous buffer from an offset.
 * \param buffer Buffer to read.
 * \param offset Stored bytes to skip, at most the stored ones.
 * \param views Stored bytes from \p offset, in order.
 * \param nviews Size of \p views on input, views filled on output.
 * \return Number of bytes described, fewer than the stored ones when \p views is too small, 0 with no view filled when \p offset is the number of stored bytes.
 * \remark Describes a second view only when the stored bytes of a ring buffer wrap around the end of the block: the bytes of a flat or dynamic buffer never do, so the same code serves them. */
size_t vmnl_net_buffer_contiguous_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews);

/**
 * \brief Drops the first stored bytes of a contiguous buffer, in O(1).
 * \param buffer Buffer read from.
 * \param nbytes Bytes to drop, at most the stored ones.
 * \remark Moves the offset past the dropped bytes, back to the start of the block when it reaches its end: a ring buffer wraps, a flat or dynamic buffer only reaches the end once empty.
 */
void vmnl_net_buffer_contiguous_consume(VmnlNetBuffer *buffer, const size_t nbytes);

/**
 * \brief Destroys a contiguous buffer and its bytes.
 * \param buffer Buffer to destroy.
 */
void vmnl_net_buffer_contiguous_destroy(VmnlNetBuffer *buffer);

/**
 * \brief Creates a flat or ring buffer.
 * \param ops Operations of the flat or ring buffer, stored in it.
 * \param capacity Bytes allocated.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p capacity is 0.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 * \remark Also creates the ring buffer: both reject a capacity of 0, then call vmnl_net_buffer_contiguous_create().
 */
VmnlNetBuffer *vmnl_net_buffer_flat_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error);

/**
 * \brief Gets the free space of a flat buffer, after its stored bytes.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, in one view.
 * \param nviews Size of \p views on input, 1 on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nviews is 0.
 * - #VMNL_NET_ENOBUFS: the free space is less than \p nbytes, even with the stored bytes moved to the front.
 * \return true on success, false otherwise.
 * \remark Moves the stored bytes to the front of the block only when the room after them lacks \p nbytes and the bytes consumed before them make up for it.
 */
bool vmnl_net_buffer_flat_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Stores the first prepared bytes of a flat or dynamic buffer.
 * \param buffer Buffer written to.
 * \param nbytes Bytes written at the start of the view of the last prepare.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than the room after the stored bytes.
 * \return true on success, false otherwise.
 * \remark Shared with the dynamic buffer: both keep their free space after their stored bytes, never before them.
 */
bool vmnl_net_buffer_flat_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error);

/**
 * \brief Grows a dynamic buffer to a capacity, exactly.
 * \param buffer Buffer to grow.
 * \param capacity Capacity to grow to, more than the current one.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Reallocates the bytes, which moves them when the allocator cannot grow the block in place.
 */
bool vmnl_net_buffer_dynamic_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Reduces the capacity of a dynamic buffer.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to, at least the stored bytes and less than the current capacity.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Moves the stored bytes to the front of the block first, so that the reallocation keeps them all.
 */
bool vmnl_net_buffer_dynamic_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Gets the free space of a dynamic buffer, growing it when needed.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, in one view.
 * \param nviews Size of \p views on input, 1 on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nviews is 0.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Moves the stored bytes to the front first when that makes enough room, and grows only otherwise, as vmnl_net_buffer_create() describes.
 */
bool vmnl_net_buffer_dynamic_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Grows a ring buffer to a capacity, exactly.
 * \param buffer Buffer to grow.
 * \param capacity Capacity to grow to, more than the current one.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Copies the stored bytes to the front of a new block, unwrapped: growing in place would leave a gap in the middle of wrapped bytes.
 */
bool vmnl_net_buffer_ring_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Reduces the capacity of a ring buffer.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to, at least the stored bytes and less than the current capacity.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Copies the stored bytes like vmnl_net_buffer_ring_resize().
 */
bool vmnl_net_buffer_ring_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Gets the free space of a ring buffer, after its stored bytes and around the end of the block.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, in order: a second view when it wraps around the end of the block.
 * \param nviews Size of \p views on input, views filled on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p views is too small to describe \p nbytes.
 * - #VMNL_NET_ENOBUFS: the free space is less than \p nbytes.
 * \return true on success, false otherwise.
 */
bool vmnl_net_buffer_ring_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Stores the first prepared bytes of a ring buffer.
 * \param buffer Buffer written to.
 * \param nbytes Bytes written at the start of the views of the last prepare.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than the free space.
 * \return true on success, false otherwise.
 * \remark The free space is the capacity minus the stored bytes, wherever it lies: the committed bytes may wrap around the end of the block.
 */
bool vmnl_net_buffer_ring_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error);

/**
 * \brief Creates a chunk buffer.
 * \param ops Operations of the chunk buffer, stored in it.
 * \param capacity Bytes allocated, rounded up to whole chunks of #VMNL_NET_BUFFER_CHUNK_SIZE bytes, 0 to allocate on the first write.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 */
VmnlNetBuffer *vmnl_net_buffer_chunked_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error);

/**
 * \brief Drops every stored byte of a chunk buffer, in O(n).
 * \param buffer Buffer to clear.
 * \remark Resets the offset and the size of every chunk, and keeps them allocated.
 */
void vmnl_net_buffer_chunked_clear(VmnlNetBuffer *buffer);

/**
 * \brief Grows a chunk buffer by whole chunks.
 * \param buffer Buffer to grow.
 * \param capacity Capacity to grow to, more than the current one.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Allocates the new chunks in a list of their own, then splices it in O(1): a failed allocation frees them and leaves the buffer as it was.
 */
bool vmnl_net_buffer_chunked_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Frees the free chunks of a chunk buffer, from the end.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to, at least the stored bytes and less than the current capacity.
 * \param error Error code of the call, nullable.
 * \return true, freeing chunks cannot fail.
 * \attention A chunk holding bytes **MUST** be kept: when the first chunk starts at an offset, the stored bytes span more chunks than \p capacity rounds up to.
 */
bool vmnl_net_buffer_chunked_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Gets the stored bytes of a chunk buffer from an offset.
 * \param buffer Buffer to read.
 * \param offset Stored bytes to skip, fewer than the stored ones.
 * \param views Stored bytes from \p offset, one view per chunk, in order.
 * \param nviews Size of \p views on input, views filled on output.
 * \return Number of bytes described, fewer than the stored ones when \p views is too small.
 */
size_t vmnl_net_buffer_chunked_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews);

/**
 * \brief Drops the first stored bytes of a chunk buffer.
 * \param buffer Buffer read from.
 * \param nbytes Bytes to drop, at most the stored ones.
 * \attention A chunk it empties **MUST** move to the end of the list with its offset back to 0: vmnl_net_buffer_chunked_commit() and vmnl_net_buffer_chunked_shrink() rely on the stored bytes starting in the first chunk and the empty chunks coming last.
 */
void vmnl_net_buffer_chunked_consume(VmnlNetBuffer *buffer, const size_t nbytes);

/**
 * \brief Gets the free space of a chunk buffer, adding chunks when needed.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, one view per chunk, in order.
 * \param nviews Size of \p views on input, views filled on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p views is too small to describe \p nbytes.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 */
bool vmnl_net_buffer_chunked_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Stores the first prepared bytes of a chunk buffer.
 * \param buffer Buffer written to.
 * \param nbytes Bytes written at the start of the views of the last prepare.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than the free space.
 * \return true on success, false otherwise.
 * \remark The free space leaves out the bytes consumed from the first chunk: they stay unusable until the chunk is recycled.
 */
bool vmnl_net_buffer_chunked_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error);

/**
 * \brief Destroys a chunk buffer and its chunks.
 * \param buffer Buffer to destroy.
 */
void vmnl_net_buffer_chunked_destroy(VmnlNetBuffer *buffer);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_BUFFER_INTERNAL_H */
