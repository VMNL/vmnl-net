#ifndef VMNL_NET_HANDLE_H
#define VMNL_NET_HANDLE_H

#ifdef _WIN32
#include <stdint.h>

typedef uintptr_t VmnlNetHandle;
#else
typedef int VmnlNetHandle;
#endif

#endif /* !VMNL_NET_HANDLE_H */
