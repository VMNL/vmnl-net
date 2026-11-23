#include "address/address_internal.h"

#include "utest.h"

UTEST(vmnl_net_address_as_sockaddr, converts_ipv4_address_for_ipv4_socket)
{
    const uint8_t expected[4] = {192, 0, 2, 1};
    struct sockaddr_storage storage;
    struct sockaddr_in ipv4;
    socklen_t socklen = 0;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_as_sockaddr(&address, AF_INET, &storage, &socklen));
    memcpy(&ipv4, &storage, sizeof ipv4);
    EXPECT_EQ(socklen, (socklen_t) sizeof(struct sockaddr_in));
    EXPECT_EQ(ipv4.sin_family, AF_INET);
    EXPECT_EQ(ipv4.sin_port, htons(8080));
    EXPECT_EQ(memcmp(&ipv4.sin_addr, expected, sizeof expected), 0);
}

UTEST(vmnl_net_address_as_sockaddr, keeps_ipv4_address_mapped_for_ipv6_socket)
{
    const uint8_t expected[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1};
    struct sockaddr_storage storage;
    struct sockaddr_in6 ipv6;
    socklen_t socklen = 0;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_as_sockaddr(&address, AF_INET6, &storage, &socklen));
    memcpy(&ipv6, &storage, sizeof ipv6);
    EXPECT_EQ(socklen, (socklen_t) sizeof(struct sockaddr_in6));
    EXPECT_EQ(ipv6.sin6_family, AF_INET6);
    EXPECT_EQ(memcmp(&ipv6.sin6_addr, expected, sizeof expected), 0);
}

UTEST(vmnl_net_address_as_sockaddr, converts_ipv6_address)
{
    const uint8_t expected[16] = {0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    struct sockaddr_storage storage;
    struct sockaddr_in6 ipv6;
    socklen_t socklen = 0;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_as_sockaddr(&address, AF_INET6, &storage, &socklen));
    memcpy(&ipv6, &storage, sizeof ipv6);
    EXPECT_EQ(socklen, (socklen_t) sizeof(struct sockaddr_in6));
    EXPECT_EQ(ipv6.sin6_family, AF_INET6);
    EXPECT_EQ(ipv6.sin6_port, htons(8080));
    EXPECT_EQ(memcmp(&ipv6.sin6_addr, expected, sizeof expected), 0);
    EXPECT_EQ(ipv6.sin6_flowinfo, (uint32_t) 0);
}

UTEST(vmnl_net_address_as_sockaddr, keeps_scope)
{
    struct sockaddr_storage storage;
    struct sockaddr_in6 ipv6;
    socklen_t socklen = 0;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("fe80::1", 8080, &address, NULL));
    address.scope = 3;
    EXPECT_TRUE(vmnl_net_address_as_sockaddr(&address, AF_INET6, &storage, &socklen));
    memcpy(&ipv6, &storage, sizeof ipv6);
    EXPECT_EQ(ipv6.sin6_scope_id, (uint32_t) 3);
}

UTEST(vmnl_net_address_as_sockaddr, writes_port_in_network_byte_order)
{
    struct sockaddr_storage storage;
    struct sockaddr_in ipv4;
    socklen_t socklen = 0;
    VmnlNetAddress address;
    uint8_t port[2];

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 0x1234, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_as_sockaddr(&address, AF_INET, &storage, &socklen));
    memcpy(&ipv4, &storage, sizeof ipv4);
    memcpy(port, &ipv4.sin_port, sizeof port);
    EXPECT_EQ(port[0], 0x12);
    EXPECT_EQ(port[1], 0x34);
}

UTEST(vmnl_net_address_as_sockaddr, rejects_ipv6_address_for_ipv4_socket)
{
    struct sockaddr_storage storage;
    socklen_t socklen = 0;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_as_sockaddr(&address, AF_INET, &storage, &socklen));
    EXPECT_EQ(socklen, (socklen_t) 0);
}

UTEST(vmnl_net_address_as_sockaddr, rejects_other_family)
{
    struct sockaddr_storage storage;
    socklen_t socklen = 0;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_as_sockaddr(&address, AF_UNIX, &storage, &socklen));
    EXPECT_EQ(socklen, (socklen_t) 0);
}
