#include "address/address_internal.h"

#include "utest.h"

UTEST(vmnl_net_address_from_sockaddr, maps_ipv4_socket_address)
{
    const uint8_t bytes[4] = {192, 0, 2, 1};
    const uint8_t expected[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1};
    struct sockaddr_storage storage = {0};
    struct sockaddr_in ipv4 = {0};
    VmnlNetAddress address = {.scope = 3};

    ipv4.sin_family = AF_INET;
    ipv4.sin_port = htons(8080);
    memcpy(&ipv4.sin_addr, bytes, sizeof bytes);
    memcpy(&storage, &ipv4, sizeof ipv4);
    EXPECT_TRUE(vmnl_net_address_from_sockaddr(&storage, &address));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
    EXPECT_EQ(address.port, 8080);
    EXPECT_EQ(address.scope, (uint32_t) 0);
}

UTEST(vmnl_net_address_from_sockaddr, converts_ipv6_socket_address)
{
    const uint8_t expected[16] = {0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    struct sockaddr_storage storage = {0};
    struct sockaddr_in6 ipv6 = {0};
    VmnlNetAddress address;

    ipv6.sin6_family = AF_INET6;
    ipv6.sin6_port = htons(8080);
    memcpy(&ipv6.sin6_addr, expected, sizeof expected);
    ipv6.sin6_scope_id = 3;
    memcpy(&storage, &ipv6, sizeof ipv6);
    EXPECT_TRUE(vmnl_net_address_from_sockaddr(&storage, &address));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
    EXPECT_EQ(address.port, 8080);
    EXPECT_EQ(address.scope, (uint32_t) 3);
}

UTEST(vmnl_net_address_from_sockaddr, keeps_mapped_ipv6_socket_address)
{
    const uint8_t bytes[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1};
    struct sockaddr_storage storage = {0};
    struct sockaddr_in6 ipv6 = {0};
    VmnlNetAddress address;

    ipv6.sin6_family = AF_INET6;
    memcpy(&ipv6.sin6_addr, bytes, sizeof bytes);
    memcpy(&storage, &ipv6, sizeof ipv6);
    EXPECT_TRUE(vmnl_net_address_from_sockaddr(&storage, &address));
    EXPECT_EQ(vmnl_net_address_version(&address), VMNL_NET_ADDRESS_IPV4);
}

UTEST(vmnl_net_address_from_sockaddr, reads_port_in_network_byte_order)
{
    const uint8_t port[2] = {0x12, 0x34};
    struct sockaddr_storage storage = {0};
    struct sockaddr_in ipv4 = {0};
    VmnlNetAddress address;

    ipv4.sin_family = AF_INET;
    memcpy(&ipv4.sin_port, port, sizeof port);
    memcpy(&storage, &ipv4, sizeof ipv4);
    EXPECT_TRUE(vmnl_net_address_from_sockaddr(&storage, &address));
    EXPECT_EQ(address.port, 0x1234);
}

UTEST(vmnl_net_address_from_sockaddr, returns_same_address_after_as_sockaddr)
{
    struct sockaddr_storage storage = {0};
    socklen_t socklen = 0;
    VmnlNetAddress address;
    VmnlNetAddress converted;

    ASSERT_TRUE(vmnl_net_address_make("fe80::1", 8080, &address, NULL));
    address.scope = 3;
    ASSERT_TRUE(vmnl_net_address_as_sockaddr(&address, AF_INET6, &storage, &socklen));
    ASSERT_TRUE(vmnl_net_address_from_sockaddr(&storage, &converted));
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &converted));
}

UTEST(vmnl_net_address_from_sockaddr, rejects_other_family)
{
    struct sockaddr_storage storage = {0};
    VmnlNetAddress address;
    VmnlNetAddress before;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    before = address;
    storage.ss_family = AF_UNIX;
    EXPECT_FALSE(vmnl_net_address_from_sockaddr(&storage, &address));
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &before));
}
