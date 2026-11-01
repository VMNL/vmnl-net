#include "buffer/buffer_internal.h"
#include "error/error_internal.h"

bool vmnl_net_buffer_chunked_commit(VmnlNetBuffer *buffer, size_t nbytes, VmnlNetError *error)
{
    struct VmnlNetBufferChunked *chunked_buffer = (struct VmnlNetBufferChunked *) buffer;
    struct VmnlNetBufferChunk *chunk = vmnl_net_buffer_chunked_writing_chunk(chunked_buffer);
    size_t chunk_committed_bytes;

    if (nbytes > vmnl_net_buffer_chunked_free_space(buffer)) {
        vmnl_net_error_set(error, VMNL_NET_EINVAL);
        return false;
    }
    VMNL_NET_LIST_FOREACH_FROM(chunk, &chunked_buffer->chunks, link)
    {
        if (nbytes == 0) {
            break;
        }
        chunk_committed_bytes = (nbytes < vmnl_net_buffer_chunk_free_space(chunk)) ? nbytes : vmnl_net_buffer_chunk_free_space(chunk);
        nbytes -= chunk_committed_bytes;
        chunk->size += chunk_committed_bytes;
        buffer->size += chunk_committed_bytes;
        chunked_buffer->commit_tail = chunk;
    }
    vmnl_net_error_set(error, VMNL_NET_SUCCESS);
    return true;
}
