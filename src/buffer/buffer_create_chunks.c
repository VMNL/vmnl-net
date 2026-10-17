#include "allocator/allocator_internal.h"
#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

#include <stdint.h>

VmnlNetBuffer *vmnl_net_buffer_create_chunks(const size_t nchunks, const size_t chunk_size, VmnlNetError *error)
{
    (void) nchunks;
    (void) chunk_size;
    (void) error;
    return NULL;
}
