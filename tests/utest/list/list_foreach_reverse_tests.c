#include "list_tests.h"

#include "utest.h"

UTEST(VMNL_NET_LIST_FOREACH_REVERSE, visits_structures_backwards)
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
    VMNL_NET_LIST_FOREACH_REVERSE(player, &team, link)
    {
        ids[visits++] = player->id;
    }
    EXPECT_EQ((int) visits, 4);
    EXPECT_EQ(memcmp(ids, (int[]) {3, 2, 1, 0}, sizeof ids), 0);
}

UTEST(VMNL_NET_LIST_FOREACH_REVERSE, removes_current_structure)
{
    struct Player players[5] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}};
    struct VmnlNetListLink team;
    struct Player *player;
    size_t visits = 0;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 5; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    VMNL_NET_LIST_FOREACH_REVERSE(player, &team, link)
    {
        if (player->id % 2 == 0) {
            vmnl_net_list_remove(&player->link);
        }
        visits++;
    }
    EXPECT_EQ((int) visits, 5);
    EXPECT_TRUE(check_list_links(&team, (int[]) {1, 3}, 2));
}

UTEST(VMNL_NET_LIST_FOREACH_REVERSE, visits_nothing_in_empty_list)
{
    struct VmnlNetListLink team;
    struct Player *player;
    size_t visits = 0;

    vmnl_net_list_init(&team);
    VMNL_NET_LIST_FOREACH_REVERSE(player, &team, link)
    {
        visits++;
    }
    EXPECT_EQ((int) visits, 0);
}
