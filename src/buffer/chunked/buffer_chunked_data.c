#include "buffer/buffer_internal.h"

size_t vmnl_net_buffer_chunked_data(const VmnlNetBuffer *buffer, const size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    struct VmnlNetBufferChunk *chunk;
    size_t nbytes_to_skip = offset;
    size_t written_views = 0;
    size_t nbytes = 0;

    VMNL_NET_LIST_FOREACH(chunk, &((const struct VmnlNetBufferChunked *) buffer)->chunks, link)
    {
        if (written_views == *nviews || chunk->size == 0) {
            break;
        }
        if (nbytes_to_skip >= chunk->size) {
            nbytes_to_skip -= chunk->size;
            continue;
        }
        views[written_views].data = chunk->bytes + chunk->offset + nbytes_to_skip;
        views[written_views].size = chunk->size - nbytes_to_skip;
        nbytes += views[written_views].size;
        nbytes_to_skip = 0;
        written_views++;
    }
    *nviews = written_views;
    return nbytes;
}
