#include "buffer/dynamic_buffer.h"

#include <stdlib.h>
#include <string.h>

#define __IMPL(buffer)          ((struct __private_dynamic_buffer *) buffer->__private)
#define __BASE_ALLOC_SIZE       1024

struct __private_dynamic_buffer
{
    bool (*__push)(const buffer_t *, const void *, size_t);
    void (*__pop)(const buffer_t *, size_t);

    size_t __size;
    size_t __offset;
    size_t __threshold;
    unsigned char *__pointer;
};

bool dynamic_buffer_resize
(const buffer_t *buffer, size_t new_size)
{
    void *pointer;

    if (new_size == 0)
    {
        return false;
    }
    if (__IMPL(buffer)->__threshold != NO_THRESHOLD && new_size > __IMPL(buffer)->__threshold)
    {
        return false;
    }
    pointer = realloc(__IMPL(buffer)->__pointer, new_size);
    if (pointer == NULL)
    {
        return false;
    }
    if (__IMPL(buffer)->__size == 0)
    {
        memset(pointer, 0, new_size);
    }
    __IMPL(buffer)->__pointer = pointer;
    __IMPL(buffer)->__size = new_size;
    if (__IMPL(buffer)->__offset > new_size)
    {
        __IMPL(buffer)->__offset = new_size;
    }
    return true;
}

bool dynamic_buffer_push
(const buffer_t *buffer, const void *data, size_t nbytes)
{
    if (__IMPL(buffer)->__threshold != NO_THRESHOLD && __IMPL(buffer)->__offset + nbytes >= __IMPL(buffer)->__threshold)
    {
        return false;
    }
    if (__IMPL(buffer)->__offset + nbytes >= __IMPL(buffer)->__size)
    {
        if (dynamic_buffer_resize(buffer, (__IMPL(buffer)->__size == 0) ? __BASE_ALLOC_SIZE : 2 * __IMPL(buffer)->__size) == false)
        {
            return false;
        }
        return dynamic_buffer_push(buffer, data, nbytes);
    }
    memmove(__IMPL(buffer)->__pointer + __IMPL(buffer)->__offset, data, nbytes);
    __IMPL(buffer)->__offset += nbytes;
    return true;
}

void dynamic_buffer_pop
(const buffer_t *buffer, size_t nbytes)
{
    if (nbytes == 0)
    {
        return;
    }
    if (nbytes >= __IMPL(buffer)->__offset)
    {
        dynamic_buffer_clear(buffer);
        return;
    }
    memmove(__IMPL(buffer)->__pointer, __IMPL(buffer)->__pointer + nbytes, __IMPL(buffer)->__offset - nbytes);
    __IMPL(buffer)->__offset -= nbytes;
    memset(__IMPL(buffer)->__pointer + __IMPL(buffer)->__offset, 0, __IMPL(buffer)->__size - __IMPL(buffer)->__offset);
}

void dynamic_buffer_clear
(const buffer_t *buffer)
{
    memset(__IMPL(buffer)->__pointer, 0, __IMPL(buffer)->__size);
    __IMPL(buffer)->__offset = 0;
}

const unsigned char *dynamic_buffer_data
(const buffer_t *buffer)
{
    return __IMPL(buffer)->__pointer;
}

size_t dynamic_buffer_size
(const buffer_t *buffer)
{
    return __IMPL(buffer)->__offset;
}

void dynamic_buffer_set_threshold
(const buffer_t *buffer, size_t threshold)
{
    __IMPL(buffer)->__threshold = threshold;
}

void dynamic_buffer_constructor
(buffer_t *buffer)
{
    __IMPL(buffer)->__push = &dynamic_buffer_push;
    __IMPL(buffer)->__pop = &dynamic_buffer_pop;
    __IMPL(buffer)->__size = 0;
    __IMPL(buffer)->__offset = 0;
    __IMPL(buffer)->__threshold = NO_THRESHOLD;
    __IMPL(buffer)->__pointer = NULL;
}
