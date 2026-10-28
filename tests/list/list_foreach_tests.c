#include "list_tests.h"

#include "utest.h"

UTEST(VMNL_NET_LIST_FOREACH, visits_structures_in_order)
{
    struct Player players[4] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}};
    struct VmnlNetListLink team;
    struct Player *player;
    int ids[4] = {0};
    size_t visits = 0;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 4; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    VMNL_NET_LIST_FOREACH(player, &team, link)
    {
        ids[visits++] = player->id;
    }
    EXPECT_EQ((int) visits, 4);
    EXPECT_EQ(memcmp(ids, (int[]) {0, 1, 2, 3}, sizeof ids), 0);
}

UTEST(VMNL_NET_LIST_FOREACH, allows_removing_current_structure)
{
    struct Player players[5] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}};
    struct VmnlNetListLink team;
    struct Player *player;
    size_t visits = 0;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 5; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    VMNL_NET_LIST_FOREACH(player, &team, link)
    {
        if (player->id % 2 == 1) {
            vmnl_net_list_remove(&player->link);
        }
        visits++;
    }
    EXPECT_EQ((int) visits, 5);
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 2, 4}, 3));
}

UTEST(VMNL_NET_LIST_FOREACH, allows_removing_every_structure)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;
    struct Player *player;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    VMNL_NET_LIST_FOREACH(player, &team, link)
    {
        vmnl_net_list_remove(&player->link);
    }
    EXPECT_TRUE(list_equals(&team, NULL, 0));
}

UTEST(VMNL_NET_LIST_FOREACH, visits_nothing_when_empty)
{
    struct VmnlNetListLink team;
    struct Player *player;
    size_t visits = 0;

    vmnl_net_list_init(&team);
    VMNL_NET_LIST_FOREACH(player, &team, link)
    {
        visits++;
    }
    EXPECT_EQ((int) visits, 0);
}
