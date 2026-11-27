/**
 * \file socket_datagram_benchmarks.hpp
 * \brief Types, sizes and sockets shared by the datagram socket benchmarks.
 */

#ifndef VMNL_NET_SOCKET_DATAGRAM_BENCHMARKS_HPP
#define VMNL_NET_SOCKET_DATAGRAM_BENCHMARKS_HPP

#include <vmnl/net/clock.h>
#include <vmnl/net/context.h>
#include <vmnl/net/socket/datagram.h>

#include <benchmark/benchmark.h>

#include <cstddef>
#include <cstdint>
#include <vector>

#define DATAGRAM_BATCH       64 /*!< Sockets that an iteration creates, binds or destroys, or datagrams that it sends or receives. */
#define DATAGRAM_SIZE        8192 /*!< Size of the largest datagram of the benchmarks. */
#define DATAGRAM_BUFFER_SIZE 1048576 /*!< Size of the system's buffers of a socket, which hold a batch of the largest datagrams. */
#define DATAGRAM_TIMEOUT     VMNL_NET_NANOSECONDS_PER_SECOND /*!< Time Sockets::receive_from() waits for each datagram. */

/**
 * \brief Type and IP version of the sockets of a benchmark, with the loopback address of the version.
 */
struct DatagramType {
    VmnlNetSocketDatagramType type; /*!< Type of the sockets. */
    VmnlNetAddressVersion version; /*!< IP version of the sockets. */
    const char *loopback; /*!< Loopback address of the version, as text. */
};

static const DatagramType udp_ipv4 = {VMNL_NET_SOCKET_DATAGRAM_UDP, VMNL_NET_ADDRESS_IPV4, "127.0.0.1"}; /*!< UDP over IPv4. */
static const DatagramType udp_ipv6 = {VMNL_NET_SOCKET_DATAGRAM_UDP, VMNL_NET_ADDRESS_IPV6, "::1"}; /*!< UDP over IPv6. */

/**
 * \brief Context and sockets of a benchmark, all of one type, destroyed with the object.
 */
class Sockets {
  public:
    /**
     * \brief Creates the context and the sockets.
     * \param state State of the benchmark, skipped with an error if a creation fails.
     * \param type Type of the sockets.
     * \param nsockets Number of sockets to create, 0 for the context alone.
     */
    Sockets(benchmark::State &state, const DatagramType &type, size_t nsockets) : context(vmnl_net_context_create(NULL)), datagram_type(type), bytes(DATAGRAM_SIZE)
    {
        if (context == NULL) {
            state.SkipWithError("vmnl_net_context_create() failed");
            return;
        }
        for (size_t it = 0; it < nsockets; it++) {
            VmnlNetSocketDatagram *netsock = vmnl_net_socket_datagram_create(type.type, type.version, NULL);

            if (netsock == NULL) {
                state.SkipWithError("vmnl_net_socket_datagram_create() failed");
                break;
            }
            sockets.push_back(netsock);
        }
    }

    /**
     * \brief Destroys the sockets, then the context.
     */
    ~Sockets()
    {
        for (const auto &netsock : sockets) {
            vmnl_net_socket_datagram_destroy(netsock);
        }
        vmnl_net_context_destroy(context);
    }

    /**
     * \name Copies and moves
     * Not copyable nor movable, as two objects would destroy the sockets twice.
     * \{
     */
    Sockets(const Sockets &) = delete;
    Sockets &operator=(const Sockets &) = delete;
    Sockets(Sockets &&) = delete;
    Sockets &operator=(Sockets &&) = delete;
    /** \} */

    /**
     * \brief Binds each socket to the loopback address on a port the system chooses, and sets the size of its system's buffers to #DATAGRAM_BUFFER_SIZE.
     * \param state State of the benchmark, skipped with an error if a bind or a setting fails.
     * \return true on success, false otherwise.
     */
    bool bind(benchmark::State &state)
    {
        for (const auto &netsock : sockets) {
            VmnlNetAddress address;

            if (vmnl_net_address_make(datagram_type.loopback, VMNL_NET_ADDRESS_ANY_PORT, &address, NULL) == false || vmnl_net_socket_datagram_bind(netsock, &address, NULL) == false || vmnl_net_socket_datagram_local_address(netsock, &address, NULL) == false) {
                state.SkipWithError("vmnl_net_socket_datagram_bind() failed");
                return false;
            }
            if (vmnl_net_socket_datagram_set_receive_buffer_size(netsock, DATAGRAM_BUFFER_SIZE, NULL) == false || vmnl_net_socket_datagram_set_send_buffer_size(netsock, DATAGRAM_BUFFER_SIZE, NULL) == false) {
                state.SkipWithError("vmnl_net_socket_datagram_set_receive_buffer_size() failed");
                return false;
            }
            addresses.push_back(address);
        }
        return true;
    }

    /**
     * \brief Sends datagrams of unwritten bytes from a socket.
     * \param state State of the benchmark, skipped with an error if a send fails.
     * \param index Index of the sending socket.
     * \param address Address of the recipient.
     * \param nbytes Size of each datagram, at most #DATAGRAM_SIZE.
     * \param ndatagrams Number of datagrams to send.
     * \return true on success, false otherwise.
     */
    bool send_to(benchmark::State &state, size_t index, const VmnlNetAddress &address, size_t nbytes, size_t ndatagrams)
    {
        VmnlNetBufferView view = {};

        view.data = bytes.data();
        view.size = (VmnlNetBufferViewSize) nbytes;
        for (size_t it = 0; it < ndatagrams; it++) {
            if (vmnl_net_socket_datagram_send_to(sockets[index], &address, &view, 1, NULL) != nbytes) {
                state.SkipWithError("vmnl_net_socket_datagram_send_to() failed");
                return false;
            }
        }
        return true;
    }

    /**
     * \brief Receives datagrams on a socket, waiting for each.
     * \param state State of the benchmark, skipped with an error if a receive fails or a datagram does not come within #DATAGRAM_TIMEOUT.
     * \param index Index of the receiving socket.
     * \param ndatagrams Number of datagrams to receive.
     * \return true on success, false otherwise.
     */
    bool receive_from(benchmark::State &state, size_t index, size_t ndatagrams)
    {
        VmnlNetBufferView view = {};
        VmnlNetAddress address;
        VmnlNetError error = VMNL_NET_EAGAIN;

        view.data = bytes.data();
        view.size = (VmnlNetBufferViewSize) bytes.size();
        for (size_t it = 0; it < ndatagrams; it++) {
            (void) vmnl_net_socket_datagram_wait_for(sockets[index], VMNL_NET_SOCKET_READABLE, DATAGRAM_TIMEOUT, NULL);
            vmnl_net_socket_datagram_receive_from(sockets[index], &view, 1, &address, &error);
            if (error != VMNL_NET_SUCCESS) {
                state.SkipWithError("vmnl_net_socket_datagram_receive_from() failed");
                return false;
            }
        }
        return true;
    }

    /**
     * \brief Gets a socket.
     * \param index Index of the socket, lower than size().
     * \return Socket at \p index.
     */
    VmnlNetSocketDatagram *operator[](size_t index) const
    {
        return sockets[index];
    }

    /**
     * \brief Gets the address of a bound socket.
     * \param index Index of the socket, lower than size(), after bind().
     * \return Address of the socket at \p index.
     */
    const VmnlNetAddress &address(size_t index) const
    {
        return addresses[index];
    }

    /**
     * \brief Counts the sockets.
     * \return Number of created sockets, fewer than asked if a creation failed.
     */
    size_t size() const
    {
        return sockets.size();
    }

  private:
    VmnlNetContext *context; /*!< Context that the sockets need. */
    DatagramType datagram_type; /*!< Type of the sockets. */
    std::vector<VmnlNetSocketDatagram *> sockets; /*!< Created sockets. */
    std::vector<VmnlNetAddress> addresses; /*!< Addresses of the sockets, after bind(). */
    std::vector<uint8_t> bytes; /*!< Bytes of the datagrams, never written nor read. */
};

#endif
