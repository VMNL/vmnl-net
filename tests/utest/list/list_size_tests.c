#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_size, counts_links)
{
    struct Player players[4] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 4; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
        EXPECT_EQ(vmnl_net_list_size(&team), it + 1);
    }
}

UTEST(vmnl_net_list_size, counts_following_removals)
{
    struct Player players[4] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 4; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[2].link);
    EXPECT_EQ(vmnl_net_list_size(&team), (size_t) 3);
    vmnl_net_list_remove(&players[0].link);
    EXPECT_EQ(vmnl_net_list_size(&team), (size_t) 2);
}

UTEST(vmnl_net_list_size, counts_empty_list)
{
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    EXPECT_EQ(vmnl_net_list_size(&team), (size_t) 0);
}
