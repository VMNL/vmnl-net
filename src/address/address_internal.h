/**
 * \file address_internal.h
 * \brief Layout of an IPv4 address mapped into IPv6, and conversions to the socket addresses of the system.
 */

/**
 * \addtogroup Address
 * @{
 */

#ifndef VMNL_NET_ADDRESS_INTERNAL_H
#define VMNL_NET_ADDRESS_INTERNAL_H

#include <vmnl/net/address.h> /* IWYU pragma: export */

#include <string.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

/**
 * \brief Index of the 4 bytes of an IPv4 address in VmnlNetAddress::ip, right after #vmnl_net_address_ipv4_prefix.
 */
#define VMNL_NET_ADDRESS_IPV4_OFFSET 12

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief First bytes of an IPv4 address mapped into IPv6, the ::ffff:0:0/96 prefix of RFC 4291.
 */
static const uint8_t vmnl_net_address_ipv4_prefix[VMNL_NET_ADDRESS_IPV4_OFFSET] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff};

/**
 * \brief Checks whether an address is the limited broadcast address, 255.255.255.255.
 * \param address Address to check.
 * \return true if \p address is 255.255.255.255, false otherwise.
 * \remark A directed broadcast address, like 192.168.1.255, gives false, as it depends on the netmask of an interface.
 */
static inline bool vmnl_net_address_is_broadcast(const VmnlNetAddress *address)
{
    static const uint8_t broadcast[4] = {0xff, 0xff, 0xff, 0xff};

    return vmnl_net_address_version(address) == VMNL_NET_ADDRESS_IPV4 && memcmp(&address->ip[VMNL_NET_ADDRESS_IPV4_OFFSET], broadcast, sizeof broadcast) == 0;
}

/**
 * \brief Converts an address into the socket address of a socket family.
 *
 * An AF_INET6 socket takes the 16 bytes as they are, an IPv4 address staying mapped, so a dual-stack socket reaches IPv4 peers.
 * An AF_INET socket takes the last 4 bytes of an IPv4 address, without the scope.
 * The port goes to network byte order, and the flow information of IPv6 stays 0, as RFC 3493 asks.
 * The socket address is built in a local struct, then copied into \p storage, so no pointer to \p storage changes its type.
 * \param address Address to convert.
 * \param family Family of the socket.
 * \param storage Socket address written, unchanged on failure.
 * \param socklen Size of the socket address written, unchanged on failure.
 * \return true on success, false if \p family is neither AF_INET nor AF_INET6, or is AF_INET with an IPv6 \p address.
 */
static inline bool vmnl_net_address_as_sockaddr(const VmnlNetAddress *address, int family, struct sockaddr_storage *storage, socklen_t *socklen)
{
    struct sockaddr_in6 ipv6;
    struct sockaddr_in ipv4;

    switch (family) {
        case AF_INET6: {
            ipv6 = (struct sockaddr_in6) {.sin6_family = AF_INET6, .sin6_port = htons(address->port), .sin6_scope_id = address->scope};
            memcpy(&ipv6.sin6_addr, address->ip, sizeof address->ip);
            memcpy(storage, &ipv6, sizeof ipv6);
            *socklen = sizeof ipv6;
            return true;
        }
        case AF_INET: {
            if (vmnl_net_address_version(address) == VMNL_NET_ADDRESS_IPV6) {
                return false;
            }
            ipv4 = (struct sockaddr_in) {.sin_family = AF_INET, .sin_port = htons(address->port)};
            memcpy(&ipv4.sin_addr, &address->ip[VMNL_NET_ADDRESS_IPV4_OFFSET], sizeof ipv4.sin_addr);
            memcpy(storage, &ipv4, sizeof ipv4);
            *socklen = sizeof ipv4;
            return true;
        }
        default:
            return false;
    }
}

/**
 * \brief Converts a socket address of the system into an address.
 *
 * An AF_INET socket address is mapped into IPv6 with a scope of 0, and an AF_INET6 one keeps its 16 bytes, mapped or not, and its scope.
 * The port goes to host byte order.
 * The socket address is copied into a local struct of its family before it is read, so no pointer to \p storage changes its type.
 * \param storage Socket address of the system, like the one accept() or recvfrom() fill.
 * \param address Address written, unchanged on failure.
 * \return true on success, false if \p storage is neither AF_INET nor AF_INET6.
 */
static inline bool vmnl_net_address_from_sockaddr(const struct sockaddr_storage *storage, VmnlNetAddress *address)
{
    struct sockaddr_in6 ipv6;
    struct sockaddr_in ipv4;

    switch (storage->ss_family) {
        case AF_INET6: {
            memcpy(&ipv6, storage, sizeof ipv6);
            *address = (VmnlNetAddress) {.port = ntohs(ipv6.sin6_port), .scope = ipv6.sin6_scope_id};
            memcpy(address->ip, &ipv6.sin6_addr, sizeof address->ip);
            return true;
        }
        case AF_INET: {
            memcpy(&ipv4, storage, sizeof ipv4);
            *address = (VmnlNetAddress) {.port = ntohs(ipv4.sin_port)};
            memcpy(address->ip, vmnl_net_address_ipv4_prefix, VMNL_NET_ADDRESS_IPV4_OFFSET);
            memcpy(&address->ip[VMNL_NET_ADDRESS_IPV4_OFFSET], &ipv4.sin_addr, sizeof ipv4.sin_addr);
            return true;
        }
        default:
            return false;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ADDRESS_INTERNAL_H */

/** @} */
