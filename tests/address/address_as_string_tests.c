#include <vmnl/net/address.h>

#include "utest.h"

UTEST(vmnl_net_address_as_string, writes_ipv4_address)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, &error));
    EXPECT_STREQ(text, "192.0.2.1");
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST(vmnl_net_address_as_string, writes_ipv6_address)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, &error));
    EXPECT_STREQ(text, "2001:db8::1");
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
}

UTEST(vmnl_net_address_as_string, writes_mapped_address_as_ipv4)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("::ffff:192.0.2.1", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "192.0.2.1");
}

UTEST(vmnl_net_address_as_string, writes_compressed_form)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("2001:0DB8:0000:0000:0000:0000:0000:0001", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "2001:db8::1");
}

UTEST(vmnl_net_address_as_string, writes_longest_address)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff");
}

UTEST(vmnl_net_address_as_string, writes_longest_ipv4_address)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("255.255.255.255", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "255.255.255.255");
}

UTEST(vmnl_net_address_as_string, writes_unspecified_ipv4_address)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("0.0.0.0", 8080, &address, NULL));
    ASSERT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "0.0.0.0");
}

UTEST(vmnl_net_address_as_string, writes_zeroed_address_as_ipv6)
{
    const VmnlNetAddress address = {0};
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];

    ASSERT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "::");
}

UTEST(vmnl_net_address_as_string, writes_ip_without_scope)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("fe80::1", 8080, &address, NULL));
    address.scope = 3;
    ASSERT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "fe80::1");
}

UTEST(vmnl_net_address_as_string, rejects_size_below_minimum)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE - 1];
    VmnlNetError error = UINT32_MAX;
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_as_string(&address, text, sizeof text, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_address_as_string, keeps_text_on_failure)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE - 1] = "unchanged";
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    ASSERT_FALSE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
    EXPECT_STREQ(text, "unchanged");
}

UTEST(vmnl_net_address_as_string, accepts_null_error)
{
    char text[VMNL_NET_ADDRESS_AS_STRING_SIZE];
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_as_string(&address, text, sizeof text, NULL));
}
