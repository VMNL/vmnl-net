#include <vmnl/net/address.h>

#include "utest.h"

UTEST(vmnl_net_address_is_ipv6, returns_true_with_ipv6_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("2001:db8::1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_ipv6(&address));
}

UTEST(vmnl_net_address_is_ipv6, returns_true_with_zeroed_address)
{
    const VmnlNetAddress address = {0};

    EXPECT_TRUE(vmnl_net_address_is_ipv6(&address));
}

UTEST(vmnl_net_address_is_ipv6, returns_true_with_ipv4_compatible_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("::192.0.2.1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_ipv6(&address));
}

UTEST(vmnl_net_address_is_ipv6, returns_true_with_ipv4_translated_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("::ffff:0:192.0.2.1", 8080, &address, NULL));
    EXPECT_TRUE(vmnl_net_address_is_ipv6(&address));
}

UTEST(vmnl_net_address_is_ipv6, returns_false_with_ipv4_address)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("192.0.2.1", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_is_ipv6(&address));
}

UTEST(vmnl_net_address_is_ipv6, returns_false_with_mapped_text_in_hex)
{
    VmnlNetAddress address;

    ASSERT_TRUE(vmnl_net_address_make("::ffff:c000:201", 8080, &address, NULL));
    EXPECT_FALSE(vmnl_net_address_is_ipv6(&address));
}
