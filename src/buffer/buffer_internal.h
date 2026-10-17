/**
 * \file buffer_internal.h
 * \brief Internal definition of the buffers.
 *
 * Every kind of buffer starts with a VmnlNetBuffer that points to its VmnlNetBufferOps: a public function checks its arguments, then calls the operation of the kind.
 *
 * \par Example
 * \code
 * bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
 * {
 *     if (nbytes > buffer->size) {
 *         vmnl_net_error_set(error, VMNL_NET_EINVAL);
 *         return false;
 *     }
 *     return buffer->ops->consume(buffer, nbytes, error);
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
 * \brief Operations of one kind of buffer.
 * \attention An operation **MUST** be called once its public function checked the arguments: it does not check them again.
 */
struct VmnlNetBufferOps {
    void (*clear)(VmnlNetBuffer *buffer); /*!< Implements vmnl_net_buffer_clear(). */
    bool (*reserve)(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error); /*!< Implements vmnl_net_buffer_reserve(). */
    bool (*shrink)(VmnlNetBuffer *buffer, const size_t capacity, VmnlNetError *error); /*!< Implements vmnl_net_buffer_shrink(). */
    size_t (*data)(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews); /*!< Implements vmnl_net_buffer_data(). */
    bool (*consume)(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error); /*!< Implements vmnl_net_buffer_consume(). */
    bool (*prepare)(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetBufferView *views, size_t *nviews, VmnlNetError *error); /*!< Implements vmnl_net_buffer_prepare(). */
    bool (*commit)(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error); /*!< Implements vmnl_net_buffer_commit(). */
    void (*destroy)(VmnlNetBuffer *buffer); /*!< Implements vmnl_net_buffer_destroy(). */
};

/**
 * \brief Common part of every kind of buffer, opaque outside the library.
 */
struct VmnlNetBuffer {
    const struct VmnlNetBufferOps *ops; /*!< Operations of the kind. */
    size_t size; /*!< Stored bytes, at most capacity. */
    size_t capacity; /*!< Allocated bytes. */
};

/**
 * \brief Flat, dynamic or ring buffer, its bytes in one block.
 * \remark The stored bytes run from offset, and in a ring buffer continue at the start of the block once they reach its end.
 */
struct VmnlNetBufferContiguous {
    struct VmnlNetBuffer base; /*!< Common part, first so that a VmnlNetBuffer pointer converts to it. */
    uint8_t *pointer; /*!< Allocated bytes. */
    size_t offset; /*!< Index of the first stored byte. */
};

/**
 * \brief Chunk buffer, its bytes in a list of chunks.
 * \remark Growing by missing bytes adds missing / chunk_size chunks, plus one for a remainder: unlike (missing + chunk_size - 1) / chunk_size, it cannot overflow.
 */
struct VmnlNetBufferChunks {
    struct VmnlNetBuffer base; /*!< Common part, first so that a VmnlNetBuffer pointer converts to it. */
    struct VmnlNetListLink chunks; /*!< Head of the chunks, from the first stored byte to the free space. */
    size_t chunk_size; /*!< Bytes of every chunk. */
};

/**
 * \brief Chunk of a chunk buffer, allocated in one block with its bytes right after it.
 */
struct VmnlNetBufferChunk {
    struct VmnlNetListLink link; /*!< Link in the chunks of its buffer. */
    uint8_t *pointer; /*!< Bytes of the chunk, (uint8_t *) (chunk + 1). */
    size_t offset; /*!< Index of the first stored byte. */
    size_t size; /*!< Stored bytes. */
};

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_BUFFER_INTERNAL_H */
