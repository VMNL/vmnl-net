/**
 * \file address.h
 *
 * \defgroup Address Address
 * \addtogroup Address
 * \brief IP addresses with a port, IPv4 and IPv6 alike.
 *
 * An address is a plain struct, copied by value, whose fields are read and written directly.
 * IPv4 and IPv6 share one type, as an IPv4 address is stored mapped into IPv6.
 *
 * \par Making an address
 * \snippet example_address_basics.c make
 *
 * \par Writing an address as text
 * \snippet example_address_basics.c as_string
 *
 * \par Comparing two addresses
 * \snippet example_address_basics.c is_equal
 *
 * \see \ref example_address_basics.c
 * @{
 */

#ifndef VMNL_NET_ADDRESS_H
#define VMNL_NET_ADDRESS_H

#include <vmnl/net/error.h>

#include <stdbool.h>
#include <stddef.h>

#define VMNL_NET_ADDRESS_AS_STRING_SIZE 46 /*!< Size of a text that holds any address, null byte included. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief IP address with its port and scope.
 * \attention Two addresses **MUST** be compared with vmnl_net_address_is_equal(), not memcmp(), as the struct has padding bytes.
 */
typedef struct VmnlNetAddress {
    uint8_t ip[16]; /*!< IPv6 address in network byte order, an IPv4 address being mapped as ::ffff:a.b.c.d. */
    uint16_t port; /*!< Port in host byte order. */
    uint32_t scope; /*!< Index of the network interface of a link-local IPv6 address, like fe80::1, 0 otherwise. */
} VmnlNetAddress;

/**
 * \brief Makes an address from the text of an IP address and a port.
 *
 * The text is an IPv4 address in dotted decimal, like 192.0.2.1, or an IPv6 address in full, like 2001:0db8:0000:0000:0000:0000:0000:0001, or compressed, like 2001:db8::1.
 * The texts 192.0.2.1 and ::ffff:192.0.2.1 make the same IPv4 address.
 * A host name, like localhost, is not resolved.
 * The scope of the address made is 0.
 * \param ip_address Text of the IP address, without brackets nor zone, like the \%1 of fe80::1\%1.
 * \param port Port in host byte order.
 * \param address Address made, unchanged on failure.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p ip_address is not the text of an IP address, has a zone, or writes an IPv4 byte with a leading zero, like 010.0.0.1.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_address_make(const char *ip_address, uint16_t port, VmnlNetAddress *address, VmnlNetError *error);

/**
 * \brief Version of the Internet Protocol of an address.
 */
typedef uint32_t VmnlNetAddressVersion;

#define VMNL_NET_ADDRESS_IPV4 UINT32_C(4) /*!< IPv4, stored mapped as ::ffff:a.b.c.d. */
#define VMNL_NET_ADDRESS_IPV6 UINT32_C(6) /*!< IPv6. */

/**
 * \brief Gets the IP version of an address.
 * \param address Address to read.
 * \return #VMNL_NET_ADDRESS_IPV4 if \p address is IPv4, mapped as ::ffff:a.b.c.d, #VMNL_NET_ADDRESS_IPV6 otherwise.
 */
VMNL_NET_API VmnlNetAddressVersion vmnl_net_address_version(const VmnlNetAddress *address);

/**
 * \brief Compares two addresses.
 * \param address First address.
 * \param other Second address.
 * \return true if both have the same IP, port and scope, false otherwise.
 */
VMNL_NET_API bool vmnl_net_address_is_equal(const VmnlNetAddress *address, const VmnlNetAddress *other);

/**
 * \brief Writes the IP of an address as text.
 *
 * An IPv4 address is written in dotted decimal, like 192.168.1.1, and an IPv6 address in its compressed form, like 2001:db8::1.
 * The port and the scope are not written.
 * \param address Address to write.
 * \param bytes Null-terminated text written, unchanged on failure.
 * \param nbytes Size of \p bytes, at least #VMNL_NET_ADDRESS_AS_STRING_SIZE.
 * \param error Error code of the call, nullable:
 * - #VMNL_NET_EINVAL: \p nbytes is less than #VMNL_NET_ADDRESS_AS_STRING_SIZE.
 * \return true on success, false otherwise.
 */
VMNL_NET_API bool vmnl_net_address_as_string(const VmnlNetAddress *address, char *bytes, size_t nbytes, VmnlNetError *error);

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ADDRESS_H */

/** @} */

/**
 * \example example_address_basics.c
 * Makes an address from its text, writes it back as text and compares it with the same address mapped into IPv6.
 */
