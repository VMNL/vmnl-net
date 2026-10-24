#include "list_tests.h"

#include "utest.h"

UTEST(VMNL_NET_LIST_BACK, returns_last_structure)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    EXPECT_EQ(VMNL_NET_LIST_BACK(&team, struct Player, link), &players[2]);
}

UTEST(VMNL_NET_LIST_BACK, follows_insertions_at_back)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
        EXPECT_EQ(VMNL_NET_LIST_BACK(&team, struct Player, link), &players[it]);
    }
}

UTEST(VMNL_NET_LIST_BACK, follows_removals_at_back)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[2].link);
    EXPECT_EQ(VMNL_NET_LIST_BACK(&team, struct Player, link), &players[1]);
    vmnl_net_list_remove(&players[1].link);
    EXPECT_EQ(VMNL_NET_LIST_BACK(&team, struct Player, link), &players[0]);
    vmnl_net_list_remove(&players[0].link);
    EXPECT_EQ(VMNL_NET_LIST_BACK(&team, struct Player, link), NULL);
}

UTEST(VMNL_NET_LIST_BACK, returns_null_for_empty_list)
{
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    EXPECT_EQ(VMNL_NET_LIST_BACK(&team, struct Player, link), NULL);
}
