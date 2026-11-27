#include "socket/system/socket_system_internal.h"

#include "utest.h"

static VmnlNetBufferView views[VMNL_NET_SOCKET_MAX_VIEWS + 1];

UTEST(vmnl_net_socket_system_check_views, accepts_int_max_bytes)
{
    uint8_t byte = 0;
    VmnlNetBufferView large[2] = {{.data = &byte, .size = (size_t) INT_MAX / 2}, {.data = &byte, .size = (size_t) INT_MAX / 2 + 1}};

    EXPECT_TRUE(vmnl_net_socket_system_check_views(large, 2, NULL));
}

UTEST(vmnl_net_socket_system_check_views, rejects_more_than_int_max_bytes)
{
    VmnlNetError error = UINT32_MAX;
    uint8_t byte = 0;
    VmnlNetBufferView large[2] = {{.data = &byte, .size = (size_t) INT_MAX / 2 + 1}, {.data = &byte, .size = (size_t) INT_MAX / 2 + 1}};

    EXPECT_FALSE(vmnl_net_socket_system_check_views(large, 2, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}

UTEST(vmnl_net_socket_system_check_views, accepts_max_views)
{
    EXPECT_TRUE(vmnl_net_socket_system_check_views(views, VMNL_NET_SOCKET_MAX_VIEWS, NULL));
}

UTEST(vmnl_net_socket_system_check_views, rejects_more_than_max_views)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_socket_system_check_views(views, VMNL_NET_SOCKET_MAX_VIEWS + 1, &error));
    EXPECT_EQ(error, VMNL_NET_EMSGSIZE);
}

UTEST(vmnl_net_socket_system_check_views, rejects_zero_views)
{
    VmnlNetError error = UINT32_MAX;

    EXPECT_FALSE(vmnl_net_socket_system_check_views(views, 0, &error));
    EXPECT_EQ(error, VMNL_NET_EINVAL);
}
