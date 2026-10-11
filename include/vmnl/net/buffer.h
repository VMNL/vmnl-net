/**
 * \file buffer.h
 *
 * \defgroup Buffer Buffer
 * \addtogroup Buffer
 * \brief FIFO byte buffers
 * @{
 */

#ifndef VMNL_NET_BUFFER_H
#define VMNL_NET_BUFFER_H

#include <vmnl/net/context.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Opaque handle to a buffer.
 * \note A dynamic buffer grows on writes, a ring buffer keeps a fixed capacity.
 * \warning Functions taking a context **MUST** receive the one that created the buffer.
 */
typedef struct VmnlNetBuffer VmnlNetBuffer;

/**
 * \brief Creates a dynamic buffer.
 * \param context Context allocating the buffer.
 * \param capacity Initial capacity in bytes, can be 0.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *context* is NULL.
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 */
VMNL_NET_API VmnlNetBuffer *vmnl_net_buffer_create_dynamic(VmnlNetContext *context, size_t capacity, VmnlNetError *error);

/**
 * \brief Creates a ring buffer.
 * \param context Context allocating the buffer.
 * \param capacity Fixed capacity in bytes.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *context* is NULL or *capacity* is 0.
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * \return Created buffer on success, NULL otherwise.
 */
VMNL_NET_API VmnlNetBuffer *vmnl_net_buffer_create_ring(VmnlNetContext *context, size_t capacity, VmnlNetError *error);

/**
 * \brief Destroys a buffer.
 * \param context Context that created *buffer*.
 * \param buffer Buffer to destroy.
 * \note Does nothing if *context* or *buffer* is NULL.
 */
VMNL_NET_API void vmnl_net_buffer_destroy(VmnlNetContext *context, VmnlNetBuffer *buffer);

/**
 * \brief Gets the number of bytes stored in a buffer.
 * \param buffer Buffer to query.
 * \return Number of stored bytes, 0 if *buffer* is NULL.
 */
VMNL_NET_API size_t vmnl_net_buffer_size(const VmnlNetBuffer *buffer);

/**
 * \brief Gets the capacity of a buffer.
 * \param buffer Buffer to query.
 * \return Capacity in bytes, 0 if *buffer* is NULL.
 */
VMNL_NET_API size_t vmnl_net_buffer_capacity(const VmnlNetBuffer *buffer);

/**
 * \brief Ensures a minimum capacity.
 * \param context Context that created *buffer*.
 * \param buffer Buffer to grow.
 * \param capacity Minimum capacity in bytes.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *context* or *buffer* is NULL.
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * - VMNL_NET_ENOBUFS: *capacity* exceeds the capacity of a ring buffer.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_buffer_reserve(VmnlNetContext *context, VmnlNetBuffer *buffer, size_t capacity, VmnlNetError *error);

/**
 * \brief Copies and removes bytes from the front of a buffer.
 * \param buffer Buffer to read from.
 * \param data Destination, nullable if *size* is 0.
 * \param size Number of bytes to read.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *buffer* is NULL, *data* is NULL with a non-zero *size*, or *size* exceeds the stored bytes.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_buffer_read(VmnlNetBuffer *buffer, void *data, size_t size, VmnlNetError *error);

/**
 * \brief Appends bytes to a buffer.
 * \param context Context that created *buffer*.
 * \param buffer Buffer to write to.
 * \param data Bytes to append, nullable if *size* is 0.
 * \param size Number of bytes to append.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *context* or *buffer* is NULL, or *data* is NULL with a non-zero *size*.
 * - VMNL_NET_ENOMEM: memory allocation failure.
 * - VMNL_NET_ENOBUFS: not enough free space in a ring buffer.
 * \return true on success, false otherwise.
 * \note Nothing is written on failure.
 */
VMNL_NET_API bool vmnl_net_buffer_write(VmnlNetContext *context, VmnlNetBuffer *buffer, const void *data, size_t size, VmnlNetError *error);

/**
 * \brief Copies bytes from the front of a buffer without removing them.
 * \param buffer Buffer to read from.
 * \param data Destination, nullable if *size* is 0.
 * \param size Number of bytes to copy.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *buffer* is NULL, *data* is NULL with a non-zero *size*, or *size* exceeds the stored bytes.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_buffer_peek(const VmnlNetBuffer *buffer, void *data, size_t size, VmnlNetError *error);

/**
 * \brief Removes bytes from the front of a buffer.
 * \param buffer Buffer to remove from.
 * \param size Number of bytes to remove.
 * \param error Error code of the call, nullable:
 * - VMNL_NET_EINVAL: *buffer* is NULL or *size* exceeds the stored bytes.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_buffer_consume(VmnlNetBuffer *buffer, size_t size, VmnlNetError *error);

/**
 * \brief Removes every byte of a buffer, keeping its capacity.
 * \param buffer Buffer to clear.
 * \note Passing a NULL *buffer* is a no-op.
 */
VMNL_NET_API void vmnl_net_buffer_clear(VmnlNetBuffer *buffer);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_BUFFER_H */

/** @} */
