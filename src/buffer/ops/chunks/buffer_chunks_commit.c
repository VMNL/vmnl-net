#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunks_commit(VmnlNetBuffer *buffer, const size_t nbytes, VmnlNetError *error)
{
    const size_t chunk_size = ((const struct VmnlNetBufferChunks *) buffer)->chunk_size;
    size_t available_bytes = 0;
    size_t remaining_bytes = nbytes;
    struct VmnlNetBufferChunk *chunk;

    VMNL_NET_LIST_FOREACH(chunk, &((struct VmnlNetBufferChunks *) buffer)->chunks, link) {
        available_bytes += chunk_size - chunk->size;
    }
    if (available_bytes < nbytes) {
        vmnl_net_error_set(error, VMNL_NET_ENOBUFS);
        return false;
    }
    VMNL_NET_LIST_FOREACH(chunk, &((struct VmnlNetBufferChunks *) buffer)->chunks, link) {
        chunk->size += ((struct VmnlNetBufferChunks *) buffer)->chunk_size - chunk->size - remaining_bytes;
    }
    return true;
}
