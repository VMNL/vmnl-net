#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_insert_after, inserts_after_head)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_insert_after(&team, &players[it].link);
    }
    EXPECT_TRUE(check_list_links(&team, (int[]) {2, 1, 0}, 3));
}

UTEST(vmnl_net_list_insert_after, inserts_after_last_link)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_insert_after(&team, &players[0].link);
    vmnl_net_list_insert_after(&players[0].link, &players[1].link);
    vmnl_net_list_insert_after(&players[1].link, &players[2].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {0, 1, 2}, 3));
}

UTEST(vmnl_net_list_insert_after, inserts_between_links)
{
    struct Player players[4] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_back(&team, &players[0].link);
    vmnl_net_list_push_back(&team, &players[3].link);
    vmnl_net_list_insert_after(&players[0].link, &players[2].link);
    vmnl_net_list_insert_after(&players[0].link, &players[1].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {0, 1, 2, 3}, 4));
}
