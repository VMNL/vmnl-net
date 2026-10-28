/**
 * \file buffer_internal.h
 * \brief Internal definition of the buffers.
 *
 * Every type of buffer starts with a VmnlNetBuffer that points to its VmnlNetBufferOps: a public function checks its arguments, then calls the operation of the type.
 * An operation whose implementations cannot fail once the arguments are checked returns nothing, and its public function writes the error code.
 * The operations fit together like blocks: one shared by a whole family takes its name, like vmnl_net_buffer_contiguous_clear() for the flat, ring and dynamic buffers, and one shared by some types keeps the name of the type that defines it, the others pointing to it or calling it, like the dynamic buffer committing with vmnl_net_buffer_flat_commit().
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

#include "allocator/allocator_internal.h"
#include "list/list_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Factor a dynamic buffer grows its capacity by, when it lacks room.
 */
#define VMNL_NET_BUFFER_REALLOCATION_FACTOR 1.5

/**
 * \brief Capacity a dynamic buffer grows to at least, so that small writes do not reallocate at each byte.
 */
#define VMNL_NET_BUFFER_MINIMUM_CAPACITY ((size_t) 256)

/**
 * \brief Bytes of every chunk of a chunked buffer, until the chunks come from a pool of the context.
 */
#define VMNL_NET_BUFFER_CHUNK_SIZE ((size_t) 16384)

/**
 * \brief Operations of one type of buffer.
 * \attention An operation **MUST** be called once its public function checked the arguments: it does not check them again.
 */
struct VmnlNetBufferOps {
    VmnlNetBuffer *(*create)(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error); /*!< Implements vmnl_net_buffer_create(), receiving this table as ops. */
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
 * \brief Flat, ring or dynamic buffer, its bytes in one block.
 * \remark The stored bytes run from offset, and wrap around the end of the block in a ring buffer.
 */
struct VmnlNetBufferContiguous {
    struct VmnlNetBuffer base; /*!< Common part, first so that a VmnlNetBuffer pointer converts to it. */
    size_t offset; /*!< Index of the first stored byte. */
    uint8_t *pointer; /*!< Allocated bytes, a block apart from the buffer: reallocating it keeps the address of the buffer, held by the caller. */
};

/**
 * \brief Chunked buffer, its bytes in a list of chunks.
 * \remark The stored bytes follow each other without gap, in the order of the list: from the offset of the first chunk, through full chunks, to the last used chunk, followed by the empty chunks.
 */
struct VmnlNetBufferChunked {
    struct VmnlNetBuffer base; /*!< Common part, first so that a VmnlNetBuffer pointer converts to it. */
    struct VmnlNetListLink chunks; /*!< Head of the chunks, from the first stored byte to the free space. */
    struct VmnlNetBufferChunk *commit_tail; /*!< Last chunk holding stored bytes, where writing starts, NULL when the buffer stores none. */
};

/**
 * \brief Chunk of a chunked buffer, allocated in one block with its bytes right after it.
 * \remark Allocated with sizeof(struct VmnlNetBufferChunk) + #VMNL_NET_BUFFER_CHUNK_SIZE bytes, and freed in one call: its bytes are not a separate allocation.
 */
struct VmnlNetBufferChunk {
    struct VmnlNetListLink link; /*!< Link in the chunks of its buffer. */
    size_t offset; /*!< Index of the first stored byte, 0 except in the first chunk. */
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
 */
VmnlNetBuffer *vmnl_net_buffer_contiguous_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error);

/**
 * \brief Drops every stored byte of a contiguous buffer, in O(1).
 * \param buffer Buffer to clear.
 */
void vmnl_net_buffer_contiguous_clear(VmnlNetBuffer *buffer);

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
 * \remark Rejects a capacity of 0, then calls vmnl_net_buffer_contiguous_create().
 */
VmnlNetBuffer *vmnl_net_buffer_flat_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error);

/**
 * \brief Describes the stored bytes of a flat or dynamic buffer from an offset.
 * \param buffer Buffer to read.
 * \param offset Stored bytes to skip, at most the stored ones.
 * \param views Stored bytes from \p offset, in one view.
 * \param nviews Number of elements of \p views on input, number of views filled on output, at most 1.
 * \return Number of bytes described, 0 with no view filled when \p views has no element or \p offset is the number of stored bytes.
 * \remark The stored bytes of a flat or dynamic buffer never wrap around the end of the block, so one view describes them all.
 */
size_t vmnl_net_buffer_flat_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews);

/**
 * \brief Drops the first stored bytes of a flat or dynamic buffer, in O(1).
 * \param buffer Buffer read from.
 * \param nbytes Bytes to drop, at most the stored ones.
 * \remark Moves the offset past the dropped bytes, without wrapping around the end of the block: the stored bytes of a flat or dynamic buffer end there at the latest.
 * \remark Moves the offset back to 0 once the buffer is empty, so that the next writes have no byte to move.
 */
void vmnl_net_buffer_flat_consume(VmnlNetBuffer *buffer, const size_t nbytes);

/**
 * \brief Describes the free space of a flat buffer, after its stored bytes.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, in one view.
 * \param nviews Number of elements of \p views on input, 1 on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nviews is 0.
 * - #VMNL_NET_ENOBUFS: the free space is less than \p nbytes, even with the stored bytes moved to the front.
 * \return true on success, false otherwise.
 * \remark Checks the free space, then calls vmnl_net_buffer_dynamic_prepare(), which never grows a buffer that has room for \p nbytes.
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
 * \remark The flat and dynamic buffers keep their free space after their stored bytes, never before them.
 */
bool vmnl_net_buffer_flat_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error);

/**
 * \brief Grows or shrinks a ring buffer, and shrinks a dynamic buffer, to a capacity, exactly.
 * \param buffer Buffer to resize.
 * \param capacity Capacity to resize to, at least the stored bytes, more than 0 and other than the current one.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Copies the stored bytes to the front of a new block, unwrapped: growing in place would leave a gap in the middle of wrapped bytes, and shrinking in place would cut them.
 */
bool vmnl_net_buffer_ring_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Reduces the capacity of a ring buffer.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to, at least the stored bytes and less than the current capacity.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p capacity is 0.
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Rejects a capacity of 0, like the creation of a ring buffer, then calls vmnl_net_buffer_ring_resize(): the operations of a ring buffer never handle an empty block.
 */
bool vmnl_net_buffer_ring_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Describes the stored bytes of a ring buffer from an offset.
 * \param buffer Buffer to read.
 * \param offset Stored bytes to skip, at most the stored ones.
 * \param views Stored bytes from \p offset, in order: a second view when they wrap around the end of the block.
 * \param nviews Number of elements of \p views on input, number of views filled on output, at most 2.
 * \return Number of bytes described, fewer than the stored ones after \p offset when \p views is too small, 0 with no view filled when \p views has no element or \p offset is the number of stored bytes.
 * \remark Calls vmnl_net_buffer_flat_data() when the stored bytes do not wrap around the end of the block, or when \p views has no element.
 * \remark Otherwise the stored bytes run to the end of the block, then from its index 0: one view when \p offset skips past the end of the block, two views otherwise, the second one starting at the index 0.
 */
size_t vmnl_net_buffer_ring_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews);

/**
 * \brief Drops the first stored bytes of a ring buffer, in O(1).
 * \param buffer Buffer read from.
 * \param nbytes Bytes to drop, at most the stored ones.
 * \remark Calls vmnl_net_buffer_flat_consume(), which moves the offset back to 0 once the buffer is empty, so that its free space fits in one view.
 * \remark Then subtracts the capacity when the offset passed the end of the block.
 */
void vmnl_net_buffer_ring_consume(VmnlNetBuffer *buffer, const size_t nbytes);

/**
 * \brief Describes the free space of a ring buffer, after its stored bytes and around the end of the block.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, in order: a second view when it wraps around the end of the block.
 * \param nviews Number of elements of \p views on input, number of views filled on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p views is too small to describe \p nbytes.
 * - #VMNL_NET_ENOBUFS: the free space is less than \p nbytes.
 * \return true on success, false otherwise.
 * \remark Never moves the stored bytes: when they wrap around or reach the end of the block, the free space lies before them, in one view.
 * \remark Otherwise the free space runs from the stored bytes to the end of the block, then from the index 0 to the first stored byte: two views, or one when \p views has one element and the first part holds \p nbytes.
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
 * \brief Grows a dynamic buffer to a capacity, exactly.
 * \param buffer Buffer to grow.
 * \param capacity Capacity to grow to, more than the current one.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Reallocates the block with vmnl_net_allocator.reallocate().
 */
bool vmnl_net_buffer_dynamic_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Reduces the capacity of a dynamic buffer.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to, at least the stored bytes and less than the current capacity.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Frees the block for a capacity of 0, the buffer being empty, and calls vmnl_net_buffer_ring_resize() otherwise.
 */
bool vmnl_net_buffer_dynamic_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Describes the free space of a dynamic buffer, growing it when needed.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, in one view.
 * \param nviews Number of elements of \p views on input, 1 on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nviews is 0.
 * - #VMNL_NET_ENOMEM: memory allocation failure, or the stored bytes and \p nbytes count past SIZE_MAX.
 * \return true on success, false otherwise.
 * \remark Grows only when moving the stored bytes to the front leaves too little room, to 1.5 times its capacity, or to the bytes needed if more, and to 256 bytes at least.
 * \remark Grows before moving the stored bytes: the reallocation copies the block anyway, and the room it adds may spare the move.
 */
bool vmnl_net_buffer_dynamic_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Creates a chunked buffer.
 * \param ops Operations of the chunked buffer, stored in it.
 * \param capacity Bytes allocated, rounded up to whole chunks of #VMNL_NET_BUFFER_CHUNK_SIZE bytes, 0 to allocate on the first write.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 * \remark Creates the buffer without chunk, then adds them with vmnl_net_buffer_chunked_resize().
 */
VmnlNetBuffer *vmnl_net_buffer_chunked_create(const struct VmnlNetBufferOps *ops, const size_t capacity, VmnlNetError *error);

/**
 * \brief Drops every stored byte of a chunked buffer, in O(n).
 * \param buffer Buffer to clear.
 * \remark Resets the offset and the size of every chunk, keeps them allocated, and sets commit_tail to NULL.
 */
void vmnl_net_buffer_chunked_clear(VmnlNetBuffer *buffer);

/**
 * \brief Grows a chunked buffer by whole chunks.
 * \param buffer Buffer to grow.
 * \param capacity Capacity to grow to, more than the current one.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_ENOMEM: memory allocation failure.
 * \return true on success, false otherwise.
 * \remark Allocates the new chunks in a list of their own, then splices it at the end in O(1): a failed allocation frees them and leaves the buffer as it was.
 */
bool vmnl_net_buffer_chunked_resize(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Frees the free chunks of a chunked buffer, from the end.
 * \param buffer Buffer to shrink.
 * \param capacity Capacity to reduce to, at least the stored bytes and less than the current capacity.
 * \param error Error code of the call, nullable.
 * \return true, freeing chunks cannot fail.
 * \attention A chunk holding bytes **MUST** be kept: when the first chunk has an offset, the stored bytes span more chunks than \p capacity rounds up to.
 */
bool vmnl_net_buffer_chunked_shrink(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error);

/**
 * \brief Describes the stored bytes of a chunked buffer from an offset.
 * \param buffer Buffer to read.
 * \param offset Stored bytes to skip, fewer than the stored ones.
 * \param views Stored bytes from \p offset, one view per chunk, in order.
 * \param nviews Number of elements of \p views on input, number of views filled on output.
 * \return Number of bytes described, fewer than the stored ones when \p views is too small, 0 with no view filled when \p views has no element.
 * \remark Skips the chunks before \p offset from the first one, in O(n), and stops at the first empty chunk: the stored bytes have no gap.
 */
size_t vmnl_net_buffer_chunked_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews);

/**
 * \brief Drops the first stored bytes of a chunked buffer.
 * \param buffer Buffer read from.
 * \param nbytes Bytes to drop, at most the stored ones, 0 when the buffer has no chunk.
 * \attention A chunk it empties **MUST** move to the end of the list with its offset back to 0: the stored bytes start in the first chunk, and the empty chunks come last.
 * \attention commit_tail **MUST** be set to NULL once the buffer is empty: the chunk it pointed to moved to the end.
 */
void vmnl_net_buffer_chunked_consume(VmnlNetBuffer *buffer, const size_t nbytes);

/**
 * \brief Describes the free space of a chunked buffer, adding chunks when needed.
 * \param buffer Buffer to write to.
 * \param nbytes Bytes needed.
 * \param views Free space, one view per chunk, in order.
 * \param nviews Number of elements of \p views on input, number of views filled on output.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p views is too small to describe \p nbytes.
 * - #VMNL_NET_ENOMEM: memory allocation failure, or the capacity and \p nbytes count past SIZE_MAX.
 * \return true on success, false otherwise.
 * \remark Starts at commit_tail, or at the first chunk when the buffer is empty: the room after its stored bytes, then the empty chunks after it.
 * \remark Counts the views needed in O(1) before adding any chunk, the chunks after commit_tail being empty, so a failure leaves the buffer as it was.
 * \remark Skips commit_tail when it is full: a view is never empty.
 */
bool vmnl_net_buffer_chunked_prepare(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error);

/**
 * \brief Stores the first prepared bytes of a chunked buffer.
 * \param buffer Buffer written to.
 * \param nbytes Bytes written at the start of the views of the last prepare.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is more than the free space.
 * \return true on success, false otherwise.
 * \remark The free space leaves out the bytes consumed from the first chunk: they stay unusable until the chunk is recycled.
 * \remark Checks \p nbytes in O(1), the stored bytes having no gap: the free space is the capacity minus the stored bytes and the offset of the first chunk.
 * \remark Fills the chunks from commit_tail, or from the first chunk when the buffer is empty, then points commit_tail to the last chunk filled.
 * \remark With chunks of 10 bytes, 9 stored and 4 already read from A, the free space is 30 - 9 - 4 = 17, and committing 12 fills B, then 5 bytes of C:
 * \code
 *            A                        B                        C
 * before  [ r r r r s s s s s s ]  [ s s s . . . . . . . ]  [ . . . . . . . . . . ]
 *                                    ^ commit_tail
 * after   [ r r r r s s s s s s ]  [ s s s s s s s s s s ]  [ s s s s s . . . . . ]
 *                                                             ^ commit_tail
 * r: read, unusable until A is recycled   s: stored   .: free
 * \endcode
 */
bool vmnl_net_buffer_chunked_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error);

/**
 * \brief Destroys a chunked buffer and its chunks.
 * \param buffer Buffer to destroy.
 */
void vmnl_net_buffer_chunked_destroy(VmnlNetBuffer *buffer);

/**
 * \brief Frees every chunk of a list.
 * \param head Head of the chunks.
 * \attention \p head **MUST** be initialized again before any use: it still points to the freed chunks.
 */
static inline void vmnl_net_buffer_chunked_destroy_internal_chunks(struct VmnlNetListLink *head)
{
    struct VmnlNetBufferChunk *chunk;

    VMNL_NET_LIST_FOREACH(chunk, head, link)
    {
        vmnl_net_allocator.deallocate(chunk);
    }
}

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_BUFFER_INTERNAL_H */
