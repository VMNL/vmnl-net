#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_push_front, pushes_into_empty_list)
{
    struct Player player = {.id = 0};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_front(&team, &player.link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {0}, 1));
}

UTEST(vmnl_net_list_push_front, pushes_before_first_link)
{
    struct Player players[4] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 4; it++) {
        vmnl_net_list_push_front(&team, &players[it].link);
    }
    EXPECT_TRUE(check_list_links(&team, (int[]) {3, 2, 1, 0}, 4));
}

UTEST(vmnl_net_list_push_front, pushes_after_removal)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_front(&team, &players[0].link);
    vmnl_net_list_push_front(&team, &players[1].link);
    vmnl_net_list_remove(&players[1].link);
    vmnl_net_list_push_front(&team, &players[2].link);
    vmnl_net_list_push_front(&team, &players[1].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {1, 2, 0}, 3));
}
