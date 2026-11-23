#include <vmnl/net/address.h>

#include "utest.h"

UTEST(vmnl_net_address_is_equal, returns_true_with_same_address)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &other, NULL));
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &other));
}

UTEST(vmnl_net_address_is_equal, returns_true_with_ipv4_and_mapped_text)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_make("::ffff:192.0.2.1", 8080, &other, NULL));
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &other));
}

UTEST(vmnl_net_address_is_equal, returns_true_with_compressed_and_full_text)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_make("2001:0db8:0000:0000:0000:0000:0000:0001", 8080, &other, NULL));
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &other));
}

UTEST(vmnl_net_address_is_equal, returns_true_with_different_padding)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    memset(&address, 0xaa, sizeof address);
    memset(&other, 0x55, sizeof other);
    memset(address.ip, 0, sizeof address.ip);
    memset(other.ip, 0, sizeof other.ip);
    address.port = 8080;
    other.port = 8080;
    address.scope = 0;
    other.scope = 0;
    ASSERT_NE(memcmp(&address, &other, sizeof address), 0);
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &other));
}

UTEST(vmnl_net_address_is_equal, returns_false_with_ipv4_and_ipv4_compatible_address)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_make("::192.0.2.1", 8080, &other, NULL));
    EXPECT_FALSE(vmnl_net_address_is_equal(&address, &other));
}

UTEST(vmnl_net_address_is_equal, returns_false_with_other_ip)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_make("2001:db8::2", 8080, &other, NULL));
    EXPECT_FALSE(vmnl_net_address_is_equal(&address, &other));
}

UTEST(vmnl_net_address_is_equal, returns_false_with_other_port)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8081, &other, NULL));
    EXPECT_FALSE(vmnl_net_address_is_equal(&address, &other));
}

UTEST(vmnl_net_address_is_equal, returns_false_with_other_scope)
{
    VmnlNetAddress address;
    VmnlNetAddress other;

    ASSERT_TRUE(vmnl_net_address_make("fe80::1", 8080, &address, NULL));
    other = address;
    other.scope = 1;
    EXPECT_FALSE(vmnl_net_address_is_equal(&address, &other));
}
