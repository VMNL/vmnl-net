#include "buffer/buffer.h"

#include <stdlib.h>

struct __private_buffer
{
    bool (*__push)(const buffer_t *, const void *, size_t);
    void (*__pop)(const buffer_t *, size_t);

    size_t __generic[3];
    unsigned char *__pointer;
};

void buffer_destructor
(const buffer_t *buffer)
{
    free(((const struct __private_buffer *) buffer->__private)->__pointer);
}
