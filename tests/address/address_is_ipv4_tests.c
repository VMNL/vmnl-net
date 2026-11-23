#include <vmnl/net/address.h>

#include "utest.h"

UTEST(vmnl_net_address_is_ipv4, returns_true_with_ipv4_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_true_with_unspecified_ipv4_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("0.0.0.0", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_true_with_mapped_text_in_hex)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("::ffff:c000:201", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_true_with_mapped_bytes)
{
    const VmnlNetAddress address = {.ip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1}, .port = 8080, .scope = 3};

    EXPECT_TRUE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_ipv6_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_zeroed_address)
{
    const VmnlNetAddress address = {0};

    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_ipv4_compatible_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("::192.0.2.1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_ipv4_translated_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("::ffff:0:192.0.2.1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_nat64_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("64:ff9b::192.0.2.1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_byte_0_set)
{
    const VmnlNetAddress address = {.ip = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1}};

    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_byte_9_set)
{
    const VmnlNetAddress address = {.ip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0xff, 0xff, 192, 0, 2, 1}};

    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_byte_10_not_ff)
{
    const VmnlNetAddress address = {.ip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xfe, 0xff, 192, 0, 2, 1}};

    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}

UTEST(vmnl_net_address_is_ipv4, returns_false_with_byte_11_not_ff)
{
    const VmnlNetAddress address = {.ip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xfe, 192, 0, 2, 1}};

    EXPECT_FALSE(vmnl_net_address_is_ipv4(&address));
}
