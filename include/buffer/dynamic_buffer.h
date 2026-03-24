/**
 * @file dynamic_buffer.h
 * @brief Defines dynamic_buffer interface.
 * @author @lszsrd
 * @version 0.1
 * @date March 24, 2026
 */

#ifndef VMNL_NETWORK_BUFFER_DYNAMIC_BUFFER_H_
#define VMNL_NETWORK_BUFFER_DYNAMIC_BUFFER_H_

#include "buffer.h"

/**
 * @define NO_THRESHOLD place holder value to specify that a dynamic buffer
 * does not have any threshold.
 */
#define NO_THRESHOLD            0

/**
 * @brief Initialize a dynamic buffer object.
 *
 * On construct, a basic_socket's handle will be in a "non initialized" state and any
 * operation beside of basic_socket_open will fail.
 *
 * @param buffer                [in] dynamic buffer object.
 */
void dynamic_buffer_constructor(buffer_t *buffer);

bool dynamic_buffer_resize(const buffer_t *buffer, size_t new_size);

bool dynamic_buffer_push(const buffer_t *buffer, const void *data, size_t nbytes);

void dynamic_buffer_pop(const buffer_t *buffer, size_t nbytes);

void dynamic_buffer_clear(const buffer_t *buffer);

const unsigned char *dynamic_buffer_data(const buffer_t *buffer);

size_t dynamic_buffer_size(const buffer_t *buffer);

void dynamic_buffer_set_threshold(const buffer_t *buffer, size_t threshold);

#endif /* !VMNL_NETWORK_BUFFER_DYNAMIC_BUFFER_H_ */
