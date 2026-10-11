#include <vmnl/net.h>

#include <stdlib.h>
#include <stdio.h>

static void *allocate(size_t size)
{
    printf("Allocating %zu bytes\n", size);
    return malloc(size);
}

static void *reallocate(void *pointer, size_t size)
{
    printf("Reallocating %p to %zu bytes\n", pointer, size);
    return realloc(pointer, size);
}

static void deallocate(void *pointer)
{
    printf("Deallocating %p\n", pointer);
    free(pointer);
}

int main(void)
{
    VmnlNetContext *standard = vmnl_net_context_create(NULL);
    VmnlNetContext *custom = vmnl_net_context_create_with_allocator(&(VmnlNetAllocator) {allocate, reallocate, deallocate}, NULL);
    // The buffer allocates through custom, which every call on it must receive
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_dynamic(custom, 64, NULL);

    vmnl_net_buffer_destroy(custom, buffer);
    vmnl_net_context_destroy(custom);
    vmnl_net_context_destroy(standard);
    return 0;
}
