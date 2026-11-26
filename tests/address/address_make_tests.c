#include <vmnl/net/address.h>

#include "utest.h"

UTEST(vmnl_net_address_make, maps_ipv4_address)
{
    const uint8_t expected[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1};
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, &error));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST(vmnl_net_address_make, makes_ipv6_address)
{
    const uint8_t expected[16] = {0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, &error));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST(vmnl_net_address_make, maps_ipv4_mapped_text)
{
    const uint8_t expected[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1};
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("::ffff:192.0.2.1", 8080, &address, NULL));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
}

UTEST(vmnl_net_address_make, maps_mapped_text_in_hex)
{
    const uint8_t expected[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 192, 0, 2, 1};
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("::ffff:c000:201", 8080, &address, NULL));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
}

UTEST(vmnl_net_address_make, accepts_uppercase_hex)
{
    const uint8_t expected[16] = {0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xab};
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("2001:DB8::AB", 8080, &address, NULL));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
}

UTEST(vmnl_net_address_make, keeps_zero_port)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 0, &address, NULL));
    EXPECT_EQ(address.port, 0);
}

UTEST(vmnl_net_address_make, keeps_highest_port)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 65535, &address, NULL));
    EXPECT_EQ(address.port, 65535);
}

UTEST(vmnl_net_address_make, sets_scope_to_zero)
{
    VmnlNetAddress address = {.scope = 3};

    ASSERT_TRUE(vmnl_net_address_make("fe80::1", 8080, &address, NULL));
    EXPECT_EQ(address.scope, (uint32_t) 0);
}

UTEST(vmnl_net_address_make, accepts_octets_up_to_255)
{
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("255.255.255.255", 8080, &address, NULL));
}

UTEST(vmnl_net_address_make, rejects_octet_above_255)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("256.0.0.0", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_incomplete_ipv4_address)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.2", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_partial_ipv4_address_ending_with_dot)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_trailing_dot)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.2.1.", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_ipv4_address_as_number)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("3221225985", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_hex_ipv4_octet)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("0xc0.0.2.1", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_leading_zero_in_first_byte)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("0192.0.2.1", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_leading_zero_in_last_byte)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.2.01", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_leading_zero_in_mapped_text)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("::ffff:192.0.02.1", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, accepts_zero_bytes)
{
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("192.0.0.0", 8080, &address, NULL));
}

UTEST(vmnl_net_address_make, accepts_leading_zeros_in_ipv6_groups)
{
    const uint8_t expected[16] = {0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("2001:0db8::0001", 8080, &address, NULL));
    EXPECT_EQ(memcmp(address.ip, expected, sizeof expected), 0);
}

UTEST(vmnl_net_address_make, rejects_leading_space)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make(" 192.0.2.1", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_trailing_space)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.2.1 ", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_two_double_colons)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("2001::db8::1", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_more_than_8_groups)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("1:2:3:4:5:6:7:8:9", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_group_above_4_digits)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("12345::1", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_trailing_colon)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("2001:db8:", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_incomplete_mapped_text)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("::ffff:192.0.2", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_garbage_after_ipv4_address)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.2.1abc", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_garbage_after_ipv6_address)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("2001:db8::1xyz", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_port_in_text)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.2.1:8080", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_ipv4_prefix_length)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("192.0.2.0/24", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_ipv6_prefix_length)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("2001:db8::/32", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_zone)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("fe80::1%1", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_zone_name)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("fe80::1%eth0", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_brackets)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("[2001:db8::1]", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_host_name)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("localhost", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, rejects_empty_text)
{
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    EXPECT_FALSE(vmnl_net_address_make("", 8080, &address, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_make, keeps_address_on_failure)
{
    VmnlNetAddress address;
    VmnlNetAddress before;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    before = address;
    ASSERT_FALSE(vmnl_net_address_make("2001:db8::zz", 9090, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_equal(&address, &before));
}

UTEST(vmnl_net_address_make, accepts_null_error)
{
    VmnlNetAddress address;

    EXPECT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
}
