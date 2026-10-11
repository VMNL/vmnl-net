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
    //! [context]
    VmnlNetContext *standard = vmnl_net_context_create(NULL);
    VmnlNetContext *custom = vmnl_net_context_create_with_allocator(&(VmnlNetAllocator) {allocate, reallocate, deallocate}, NULL);

    // ...
    // Always pass the context that created an object, and destroy the object before that context
    // ...
    vmnl_net_context_destroy(custom);
    vmnl_net_context_destroy(standard);
    //! [context]
    return 0;
}
