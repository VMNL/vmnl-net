/**
 * \file address_internal.h
 * \brief Layout of an IPv4 address mapped into IPv6.
 */

/**
 * \addtogroup Address
 * @{
 */

#ifndef VMNL_NET_ADDRESS_INTERNAL_H
#define VMNL_NET_ADDRESS_INTERNAL_H

#include <vmnl/net/address.h> /* IWYU pragma: export */

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

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_ADDRESS_INTERNAL_H */

/** @} */
