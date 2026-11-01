#include "buffer/buffer_internal.h"

size_t vmnl_net_buffer_chunked_data(const VmnlNetBuffer *buffer, size_t offset, VmnlNetBufferView *views, size_t *nviews)
{
    struct VmnlNetBufferChunk *chunk;
    size_t written_views = 0;
    size_t nbytes = 0;

    VMNL_NET_LIST_FOREACH(chunk, &((const struct VmnlNetBufferChunked *) buffer)->chunks, link)
    {
        if (written_views == *nviews || chunk->size == 0) {
            break;
        }
        if (offset >= chunk->size) {
            offset -= chunk->size;
            continue;
        }
        views[written_views].data = chunk->bytes + chunk->offset + offset;
        views[written_views].size = chunk->size - offset;
        nbytes += views[written_views].size;
        offset = 0;
        written_views++;
    }
    *nviews = written_views;
    return nbytes;
}
