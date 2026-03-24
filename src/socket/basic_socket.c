#define _GNU_SOURCE

#include "socket/basic_socket.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <netdb.h>

#define __IMPL(basic_socket)    ((struct __private_basic_socket *) basic_socket->__private)

#define __INVALID_HANDLE        -1
#define __BACKLOG_SIZE          16
#define __RECV_SIZE             1024

struct __private_buffer
{
    bool (*__push)(const buffer_t *, const void *, size_t);
    void (*__pop)(const buffer_t *, size_t);

    size_t __size;
    size_t __generic1;
    size_t __generic2;
    unsigned char *__pointer;
};

struct __private_basic_socket
{
    ssize_t (*__recv)(const basic_socket_t *basic_socket);
    ssize_t (*__send)(const basic_socket_t *basic_socket);

    int __handle;
    int __domain;
    unsigned short __port;

    struct
    {
        buffer_t in;
        buffer_t out;
    } buffer;
};

static
ssize_t __basic_socket_recv_standalone_implementation
(const basic_socket_t *basic_socket)
{
    unsigned char buffer[__RECV_SIZE] = {0};
    ssize_t nbytes = recv(__IMPL(basic_socket)->__handle, buffer, __RECV_SIZE, 0);

    if (nbytes > 0)
    {
        if (((const struct __private_buffer *) basic_socket->buffer.in->__private)->__push(basic_socket->buffer.in, buffer, nbytes) == false)
        {
            return -1;
        }
    }
    return nbytes;
}

static
ssize_t __basic_socket_send_standalone_implementation
(const basic_socket_t *basic_socket)
{
    ssize_t nbytes = send(__IMPL(basic_socket)->__handle,
        ((const struct __private_buffer *) basic_socket->buffer.out->__private)->__pointer,
        ((const struct __private_buffer *) basic_socket->buffer.out->__private)->__size,
        0
    );

    if (nbytes > 0)
    {
       ((const struct __private_buffer *) basic_socket->buffer.out->__private)->__pop(basic_socket->buffer.out, nbytes);
    }
    return true;
}

bool basic_socket_open
(const basic_socket_t *basic_socket, int domain, int type, int protocol)
{
    int __handle = socket(domain, type, protocol);

    if (__handle == -1)
    {
        return false;
    }
    if (__IMPL(basic_socket)->__handle != __INVALID_HANDLE)
    {
        basic_socket_close(basic_socket);
    }
    __IMPL(basic_socket)->__handle = __handle;
    __IMPL(basic_socket)->__domain = domain;
    return true;
}

void basic_socket_close
(const basic_socket_t *basic_socket)
{
    if (__IMPL(basic_socket)->__handle == __INVALID_HANDLE)
    {
        return;
    }
    if (close(__IMPL(basic_socket)->__handle) == -1)
    {
        return;
    }
    __IMPL(basic_socket)->__handle = __INVALID_HANDLE;
}

bool basic_socket_listen
(const basic_socket_t *basic_socket, unsigned short port)
{
    struct sockaddr_in sockaddr;

    switch (__IMPL(basic_socket)->__domain)
    {
        case AF_INET:
            sockaddr = (const struct sockaddr_in) {
                .sin_family = AF_INET,
                .sin_addr.s_addr = INADDR_ANY,
                .sin_port = htons(port),
            };
            break;
        case AF_INET6:
            return false;
        default:
            return false;
    }
    if (bind(__IMPL(basic_socket)->__handle, (struct sockaddr *) &sockaddr, sizeof sockaddr) == -1)
    {
        return false;
    }
    if (listen(__IMPL(basic_socket)->__handle, __BACKLOG_SIZE) == -1)
    {
        return false;
    }
    __IMPL(basic_socket)->__port = htons(port);
    return true;
}

bool basic_socket_connect
(const basic_socket_t *basic_socket, const char *ip, unsigned short port)
{
    struct addrinfo *host;
    char servname[16] = {0};
    bool is_connected = false;

    if (snprintf(servname, sizeof servname, "%d", port) < 0)
    {
        return false;
    }
    if (getaddrinfo(ip, servname, NULL, &host) == -1)
    {
        return false;
    }
    for (struct addrinfo *it = host; it != NULL; it = it->ai_next)
    {
        if (connect(__IMPL(basic_socket)->__handle, it->ai_addr, it->ai_addrlen) != -1)
        {
            is_connected = true;
            break;
        }
    }
    freeaddrinfo(host);
    return is_connected;
}

bool basic_socket_enqueue(const basic_socket_t *basic_socket, const char *format, ...)
{
    char *buffer = NULL;
    ssize_t nbytes;
    va_list va_list;
    bool is_pushed = false;

    va_start(va_list, format);
    nbytes = vasprintf(&buffer, format, va_list);
    va_end(va_list);
    if (nbytes > 0)
    {
        is_pushed = ((const struct __private_buffer *) basic_socket->buffer.out)->__push(basic_socket->buffer.out, buffer, nbytes);
    }
    free(buffer);
    return is_pushed;
}

bool basic_socket_recv
(const basic_socket_t *basic_socket)
{
    if (__IMPL(basic_socket)->__recv(basic_socket) <= 0)
    {
        return false;
    }
    return true;
}

bool basic_socket_send
(const basic_socket_t *basic_socket)
{
    if (__IMPL(basic_socket)->__send(basic_socket) <= 0)
    {
        return false;
    }
    return true;
}

bool basic_socket_set_option
(const basic_socket_t *basic_socket, int option)
{
    return setsockopt(__IMPL(basic_socket)->__handle, SOL_SOCKET, option, &(int) {1}, sizeof(int)) != -1;
}

void basic_socket_constructor
(basic_socket_t *basic_socket, buffer_constructor_t buffer_constructor)
{
    __IMPL(basic_socket)->__recv = &__basic_socket_recv_standalone_implementation;
    __IMPL(basic_socket)->__send = &__basic_socket_send_standalone_implementation;
    __IMPL(basic_socket)->__handle = __INVALID_HANDLE;
    buffer_constructor(&__IMPL(basic_socket)->buffer.in);
    buffer_constructor(&__IMPL(basic_socket)->buffer.out);
    basic_socket->buffer.in = &__IMPL(basic_socket)->buffer.in;
    basic_socket->buffer.out = &__IMPL(basic_socket)->buffer.out;
}

void basic_socket_destructor
(const basic_socket_t *basic_socket)
{
    buffer_destructor(&__IMPL(basic_socket)->buffer.in);
    buffer_destructor(&__IMPL(basic_socket)->buffer.out);
    basic_socket_close(basic_socket);
}
