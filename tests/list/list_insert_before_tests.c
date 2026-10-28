#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_insert_before, inserts_before_head)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_insert_before(&team, &players[it].link);
    }
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 1, 2}, 3));
}

UTEST(vmnl_net_list_insert_before, inserts_before_first_link)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_insert_before(&team, &players[2].link);
    vmnl_net_list_insert_before(&players[2].link, &players[1].link);
    vmnl_net_list_insert_before(&players[1].link, &players[0].link);
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 1, 2}, 3));
}

UTEST(vmnl_net_list_insert_before, inserts_between_links)
{
    struct Player players[4] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_back(&team, &players[0].link);
    vmnl_net_list_push_back(&team, &players[3].link);
    vmnl_net_list_insert_before(&players[3].link, &players[1].link);
    vmnl_net_list_insert_before(&players[3].link, &players[2].link);
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 1, 2, 3}, 4));
}
