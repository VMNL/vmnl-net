#ifndef VMNL_NETWORK_BUFFER_BUFFER_INTERFACE_H_
#define VMNL_NETWORK_BUFFER_BUFFER_INTERFACE_H_

#include <stdbool.h>
#include <stddef.h>

#define __BUFFER_INTERNAL                                                   \
    + sizeof(bool (*)(const void *, const void *, size_t))                  \
    + sizeof(void (*)(const void *, size_t))                                \
    + sizeof(size_t) * 3                                                    \
    + sizeof(unsigned char *)

/**
 * @struct buffer_t
 * @brief Buffer interface.
 *
 * Manages a smart buffer and serves as an interface for both dynamic and
 * cicruclar buffers.
 */
typedef struct
{
    char __private[__BUFFER_INTERNAL];          /*!< Private data used to manage object. */
} buffer_t;

/**
 * @typedef buffer_constructor_t
 * @brief Buffer constructor type definitions.
 *
 * Forces type-compability for basic_socket_construct().
 */
typedef void (*buffer_constructor_t )(buffer_t *);

/**
 * @brief Destruct a buffer object.
 *
 * On destruct, any buffer deriving from the buffer_t interface's internal.
 * pointer got freed.
 *
 * @param basic_socket          [in] basic_socket object.
 */
void buffer_destructor(const buffer_t *buffer);

#endif /* !VMNL_NETWORK_BUFFER_BUFFER_INTERFACE_H_ */
