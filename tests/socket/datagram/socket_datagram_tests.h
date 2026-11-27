/**
 * \file socket_datagram_tests.h
 * \brief Fixtures and helpers of the datagram socket tests.
 */

#ifndef VMNL_NET_SOCKET_DATAGRAM_TESTS_H
#define VMNL_NET_SOCKET_DATAGRAM_TESTS_H

#include <vmnl/net/allocator.h>
#include <vmnl/net/clock.h>
#include <vmnl/net/context.h>

#include "socket/datagram/socket_datagram_internal.h"

#include "utest.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <fcntl.h>
#endif

#define NTYPES          2 /*!< Elements of #types, the count of UTEST_I. */
#define RECEIVE_TIMEOUT VMNL_NET_NANOSECONDS_PER_SECOND /*!< Time a helper waits for a datagram, as the system may deliver it later. */
#define MILLISECOND     (VMNL_NET_NANOSECONDS_PER_SECOND / VMNL_NET_MILLISECONDS_PER_SECOND) /*!< Nanoseconds in a millisecond. */

/**
 * \brief Type and IP version of the sockets of a UTEST_I index, with addresses of this version.
 */
struct DatagramType {
    VmnlNetSocketDatagramType type; /*!< Type of the sockets. */
    VmnlNetAddressVersion version; /*!< IP version of the sockets. */
    const char *loopback; /*!< Loopback address of the version, as text. */
    const char *foreign; /*!< Address of the version kept for the documentation, which no host owns, as text. */
    const char *wildcard; /*!< Address of every interface of the version, as text. */
};

/**
 * \brief Declares the fixture of a UTEST_I suite: a context, and an unbound socket of the type at the index of the test.
 * \param SUITE Name of the suite.
 * \param TYPES Types of the sockets, one per index.
 * \remark The teardown restores the default allocator, then destroys the socket and the context.
 */
#define DATAGRAM_FIXTURE_I(SUITE, TYPES)                          \
    struct SUITE {                                                \
        const struct DatagramType *type;                          \
        VmnlNetContext *context;                                  \
        VmnlNetSocketDatagram *netsock;                           \
    };                                                            \
                                                                  \
    UTEST_I_SETUP(SUITE)                                          \
    {                                                             \
        utest_fixture->type = &TYPES[utest_index];                \
        utest_fixture->context = vmnl_net_context_create(NULL);   \
        ASSERT_NE(utest_fixture->context, NULL);                  \
        utest_fixture->netsock = create(utest_fixture->type);     \
        ASSERT_NE(utest_fixture->netsock, NULL);                  \
    }                                                             \
                                                                  \
    UTEST_I_TEARDOWN(SUITE)                                       \
    {                                                             \
        (void) utest_result;                                      \
        (void) utest_index;                                       \
        vmnl_net_allocator_set(malloc, realloc, free);            \
        vmnl_net_socket_datagram_destroy(utest_fixture->netsock); \
        vmnl_net_context_destroy(utest_fixture->context);         \
    }

/**
 * \brief Declares the fixture of a UTEST_I suite: a context, and a sender and a receiver of the type at the index of the test, each bound to the loopback address on a port the system chooses.
 * \param SUITE Name of the suite.
 * \param TYPES Types of the sockets, one per index.
 * \remark The teardown restores the default allocator, then destroys the sockets and the context.
 */
#define DATAGRAM_PAIR_FIXTURE_I(SUITE, TYPES)                                                                       \
    struct SUITE {                                                                                                  \
        const struct DatagramType *type;                                                                            \
        VmnlNetContext *context;                                                                                    \
        VmnlNetSocketDatagram *sender;                                                                              \
        VmnlNetSocketDatagram *receiver;                                                                            \
        VmnlNetAddress sender_address;                                                                              \
        VmnlNetAddress receiver_address;                                                                            \
    };                                                                                                              \
                                                                                                                    \
    UTEST_I_SETUP(SUITE)                                                                                            \
    {                                                                                                               \
        utest_fixture->type = &TYPES[utest_index];                                                                  \
        utest_fixture->context = vmnl_net_context_create(NULL);                                                     \
        ASSERT_NE(utest_fixture->context, NULL);                                                                    \
        utest_fixture->sender = create(utest_fixture->type);                                                        \
        ASSERT_NE(utest_fixture->sender, NULL);                                                                     \
        utest_fixture->receiver = create(utest_fixture->type);                                                      \
        ASSERT_NE(utest_fixture->receiver, NULL);                                                                   \
        ASSERT_TRUE(bind_loopback(utest_fixture->sender, utest_fixture->type, &utest_fixture->sender_address));     \
        ASSERT_TRUE(bind_loopback(utest_fixture->receiver, utest_fixture->type, &utest_fixture->receiver_address)); \
    }                                                                                                               \
                                                                                                                    \
    UTEST_I_TEARDOWN(SUITE)                                                                                         \
    {                                                                                                               \
        (void) utest_result;                                                                                        \
        (void) utest_index;                                                                                         \
        vmnl_net_allocator_set(malloc, realloc, free);                                                              \
        vmnl_net_socket_datagram_destroy(utest_fixture->sender);                                                    \
        vmnl_net_socket_datagram_destroy(utest_fixture->receiver);                                                  \
        vmnl_net_context_destroy(utest_fixture->context);                                                           \
    }

/**
 * \brief Every type, once per IP version, in the order of the UTEST_I indexes.
 */
static const struct DatagramType types[NTYPES] = {
    {VMNL_NET_SOCKET_DATAGRAM_UDP, VMNL_NET_ADDRESS_IPV4, "127.0.0.1", "192.0.2.1", "0.0.0.0"},
    {VMNL_NET_SOCKET_DATAGRAM_UDP, VMNL_NET_ADDRESS_IPV6, "::1", "2001:db8::1", "::"},
};

/**
 * \brief Checks whether the child processes inherit a socket, which each system reads its own way.
 * \param handle Native handle of the socket.
 * \return true if a child process inherits \p handle, false otherwise.
 */
static inline bool is_inherited(VmnlNetHandle handle)
{
#ifdef _WIN32
    DWORD flags = 0;

    /* NOLINTNEXTLINE(performance-no-int-to-ptr) */
    return GetHandleInformation((HANDLE) handle, &flags) != false && (flags & HANDLE_FLAG_INHERIT) != 0;
#else
    return ((unsigned int) fcntl(handle, F_GETFD) & (unsigned int) FD_CLOEXEC) == 0;
#endif
}

/**
 * \brief Operations of a type without broadcast nor system buffer, for the cases no real type triggers.
 */
static const struct VmnlNetSocketDatagramOps unsupported_ops = {0};

/**
 * \brief Creates a datagram socket.
 * \param type Type and IP version of the socket.
 * \return Created socket on success, NULL otherwise.
 */
static inline VmnlNetSocketDatagram *create(const struct DatagramType *type)
{
    return vmnl_net_socket_datagram_create(type->type, type->version, NULL);
}

/**
 * \brief Binds a datagram socket to the loopback address of its type, on a port the system chooses.
 * \param netsock Socket to bind.
 * \param type Type of \p netsock.
 * \param address Address \p netsock is bound to, with its port.
 * \return true on success, false otherwise.
 */
static inline bool bind_loopback(VmnlNetSocketDatagram *netsock, const struct DatagramType *type, VmnlNetAddress *address)
{
    return vmnl_net_address_make(type->loopback, VMNL_NET_ADDRESS_ANY_PORT, address, NULL) == true && vmnl_net_socket_datagram_bind(netsock, address, NULL) == true && vmnl_net_socket_datagram_local_address(netsock, address, NULL) == true;
}

/**
 * \brief Points views to the same byte, to pass more views than a call accepts.
 * \param views Views to set.
 * \param nviews Number of elements of \p views.
 * \param byte Byte that every view describes.
 */
static inline void views_on_byte(VmnlNetBufferView *views, size_t nviews, uint8_t *byte)
{
    for (size_t it = 0; it < nviews; it++) {
        views[it] = (VmnlNetBufferView) {.data = byte, .size = 1};
    }
}

/**
 * \brief Sends the characters of a string as one datagram.
 * \param netsock Socket to send on.
 * \param address Address of the recipient.
 * \param bytes String to send, without its null character.
 * \return true if the whole string is sent, false otherwise.
 */
static inline bool send_to(VmnlNetSocketDatagram *netsock, const VmnlNetAddress *address, const char *bytes)
{
    size_t nbytes = strlen(bytes);
    VmnlNetBufferView view = {.data = (uint8_t *) bytes, .size = (VmnlNetBufferViewSize) nbytes};
    VmnlNetError error = UINT32_MAX;

    return vmnl_net_socket_datagram_send_to(netsock, address, &view, 1, &error) == nbytes && error == VMNL_NET_SUCCESS;
}

/**
 * \brief Waits for a datagram for #RECEIVE_TIMEOUT at most, then calls vmnl_net_socket_datagram_receive().
 * \param netsock Connected socket to receive from.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param error Error code of the receive, #VMNL_NET_EAGAIN if nothing came.
 * \return Result of the receive.
 */
static inline size_t receive_waiting(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetError *error)
{
    (void) vmnl_net_socket_datagram_wait_for(netsock, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, NULL);
    return vmnl_net_socket_datagram_receive(netsock, views, nviews, error);
}

/**
 * \brief Waits for a datagram for #RECEIVE_TIMEOUT at most, then calls vmnl_net_socket_datagram_receive_from().
 * \param netsock Socket to receive from.
 * \param views Space to fill, in order.
 * \param nviews Number of elements of \p views.
 * \param address Address of the sender.
 * \param error Error code of the receive, #VMNL_NET_EAGAIN if nothing came.
 * \return Result of the receive.
 */
static inline size_t receive_from_waiting(VmnlNetSocketDatagram *netsock, const VmnlNetBufferView *views, size_t nviews, VmnlNetAddress *address, VmnlNetError *error)
{
    (void) vmnl_net_socket_datagram_wait_for(netsock, VMNL_NET_SOCKET_READABLE, RECEIVE_TIMEOUT, NULL);
    return vmnl_net_socket_datagram_receive_from(netsock, views, nviews, address, error);
}

/**
 * \brief Waits for one datagram, and compares it with a string.
 * \param netsock Socket to receive from.
 * \param bytes Expected datagram, as a string of at most 64 characters.
 * \return true if a datagram comes within #RECEIVE_TIMEOUT and holds the characters of \p bytes, false otherwise.
 */
static inline bool datagram_equals(VmnlNetSocketDatagram *netsock, const char *bytes)
{
    uint8_t received[64];
    VmnlNetAddress address;
    VmnlNetError error = UINT32_MAX;
    size_t nbytes = receive_from_waiting(netsock, &(VmnlNetBufferView) {.data = received, .size = 64}, 1, &address, &error);

    return error == VMNL_NET_SUCCESS && nbytes == strlen(bytes) && memcmp(received, bytes, nbytes) == 0;
}

#endif /* !VMNL_NET_SOCKET_DATAGRAM_TESTS_H */
