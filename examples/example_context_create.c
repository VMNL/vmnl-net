#include <vmnl/net/context.h>

#include <stdio.h>

int main(void)
{
    //! [create]
    VmnlNetError error;
    VmnlNetContext *context = vmnl_net_context_create(&error);

    if (context == NULL) {
        fprintf(stderr, "%s\n", vmnl_net_error_as_string(error)); // Cannot allocate memory
        return 1;
    }
    // The timers of this thread start in this context.
    vmnl_net_context_destroy(context);
    //! [create]
    return 0;
}
