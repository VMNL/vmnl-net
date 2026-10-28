#include "list_tests.h"

#include "utest.h"

UTEST(VMNL_NET_LIST_FRONT, returns_first_structure)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    EXPECT_EQ(VMNL_NET_LIST_FRONT(&team, struct Player, link), &players[0]);
}

UTEST(VMNL_NET_LIST_FRONT, follows_insertions_at_front)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_front(&team, &players[it].link);
        EXPECT_EQ(VMNL_NET_LIST_FRONT(&team, struct Player, link), &players[it]);
    }
}

UTEST(VMNL_NET_LIST_FRONT, follows_removals_at_front)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[0].link);
    EXPECT_EQ(VMNL_NET_LIST_FRONT(&team, struct Player, link), &players[1]);
    vmnl_net_list_remove(&players[1].link);
    EXPECT_EQ(VMNL_NET_LIST_FRONT(&team, struct Player, link), &players[2]);
    vmnl_net_list_remove(&players[2].link);
    EXPECT_EQ(VMNL_NET_LIST_FRONT(&team, struct Player, link), NULL);
}

UTEST(VMNL_NET_LIST_FRONT, returns_null_when_empty)
{
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    EXPECT_EQ(VMNL_NET_LIST_FRONT(&team, struct Player, link), NULL);
}
