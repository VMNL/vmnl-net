#include <vmnl/net/allocator.h>
#include <vmnl/net/buffer.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Number of blocks allocated and not released yet.
static size_t nblocks = 0;

static void *malloc_debug(size_t size)
{
    void *pointer = malloc(size);

    if (pointer != NULL) {
        nblocks++;
    }
    return pointer;
}

static void *realloc_debug(void *pointer, size_t size)
{
    void *result = realloc(pointer, size);

    // realloc() allocates a new block when the pointer is NULL.
    if (pointer == NULL && result != NULL) {
        nblocks++;
    }
    return result;
}

static void free_debug(void *pointer)
{
    if (pointer != NULL) {
        nblocks--;
    }
    free(pointer);
}

int main(void)
{
    //! [set]
    // Before any other call to the library
    vmnl_net_allocator_set(malloc_debug, realloc_debug, free_debug);
    //! [set]

    VmnlNetBuffer *buffer = vmnl_net_buffer_create(VMNL_NET_BUFFER_FLAT, 64, NULL);

    if (buffer == NULL) {
        return 1;
    }
    printf("%zu blocks allocated\n", nblocks);
    vmnl_net_buffer_destroy(buffer);
    printf("%zu blocks left after the destruction\n", nblocks);
    return nblocks != 0;
}
