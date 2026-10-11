/**
 * \file buffer_internal.h
 * \brief Internal definition and helpers of the buffers.
 */

#ifndef VMNL_NET_BUFFER_INTERNAL_H
#define VMNL_NET_BUFFER_INTERNAL_H

#include <vmnl/net/buffer.h> /* IWYU pragma: export */

/**
 * \brief Kinds of buffer.
 */
typedef enum {
    VMNL_NET_BUFFER_TYPE_DYNAMIC, /*!< Grows when full. */
    VMNL_NET_BUFFER_TYPE_RING, /*!< Fixed capacity, wraps around. */
} VmnlNetBufferType;

/**
 * \brief Internal definition of a buffer.
 */
struct VmnlNetBuffer {
    uint8_t *data; /*!< Storage, NULL while the capacity is 0. */
    size_t capacity; /*!< Storage size in bytes. */
    size_t size; /*!< Number of stored bytes. */
    size_t start; /*!< Index of the first stored byte. */
    VmnlNetBufferType type; /*!< Kind of buffer. */
};

/**
 * \brief Region of the storage of a buffer.
 * \warning Invalidated by any modification of the buffer.
 */
typedef struct VmnlNetBufferView {
    uint8_t *data; /*!< First byte. */
    size_t size; /*!< Number of bytes, 0 if unused. */
} VmnlNetBufferView;

/**
 * \brief Creates a buffer of the given kind.
 * \param context Context allocating the buffer, **MUST** not be NULL.
 * \param type Kind of buffer.
 * \param capacity Initial capacity in bytes, can be 0.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 */
VmnlNetBuffer *vmnl_net_buffer_create_generic(VmnlNetContext *context, VmnlNetBufferType type, size_t capacity, VmnlNetError *error);

/**
 * \brief Resizes the storage of a buffer.
 * \param context Context that created *buffer*.
 * \param buffer Buffer to resize.
 * \param capacity New capacity in bytes, **MUST** not be 0.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_ENOMEM: memory allocation failure, the storage is unchanged.
 * \return true on success, false otherwise.
 */
bool vmnl_net_buffer_resize(VmnlNetContext *context, VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error);

/**
 * \brief Frees the storage of a buffer.
 * \param context Context that created *buffer*.
 * \param buffer Buffer to release.
 */
void vmnl_net_buffer_release(VmnlNetContext *context, VmnlNetBuffer *buffer);

/**
 * \brief Gets the stored bytes of a buffer without copying them.
 * \param buffer Buffer to view.
 * \param views Stored bytes in order, *views[1]* is empty unless a ring wraps around.
 */
void vmnl_net_buffer_view(const VmnlNetBuffer *buffer, VmnlNetBufferView views[2]);

/**
 * \brief Gets at least *size* bytes of free space to write in place.
 * \param context Context that created *buffer*, nullable for a ring.
 * \param buffer Buffer to write to.
 * \param size Number of bytes needed.
 * \param views Free space in order, *views[1]* is empty unless a ring wraps around.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * - VMNL_NET_ENOBUFS: not enough free space in a ring buffer.
 * \return true on success, false otherwise.
 * \note A dynamic buffer moves its bytes to the front or grows to make room.
 */
bool vmnl_net_buffer_claim(VmnlNetContext *context, VmnlNetBuffer *buffer, size_t size, VmnlNetBufferView views[2], VmnlNetError *error);

/**
 * \brief Makes bytes written in the claimed space readable.
 * \param buffer Buffer written to.
 * \param size Number of bytes written, **MUST** not exceed the claimed space.
 */
void vmnl_net_buffer_commit(VmnlNetBuffer *buffer, size_t size);

/**
 * \brief Removes bytes from the front of a buffer.
 * \param buffer Buffer to remove from.
 * \param size Number of bytes, **MUST** not exceed the stored bytes.
 */
void vmnl_net_buffer_discard(VmnlNetBuffer *buffer, size_t size);

/**
 * \brief Copies bytes from the front of a buffer without removing them.
 * \param buffer Buffer to copy from.
 * \param data Destination.
 * \param size Number of bytes, **MUST** not exceed the stored bytes.
 */
void vmnl_net_buffer_copy(const VmnlNetBuffer *buffer, void *data, size_t size);

#endif /* !VMNL_NET_BUFFER_INTERNAL_H */
