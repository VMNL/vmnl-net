#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_rotate_left, moves_first_link_to_end)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_rotate_left(&team);
    EXPECT_TRUE(list_equals(&team, (int[]) {1, 2, 0}, 3));
}

UTEST(vmnl_net_list_rotate_left, keeps_single_link)
{
    struct Player player = {.id = 0};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_back(&team, &player.link);
    vmnl_net_list_rotate_left(&team);
    EXPECT_TRUE(list_equals(&team, (int[]) {0}, 1));
}

UTEST(vmnl_net_list_rotate_left, ignores_empty_list)
{
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_rotate_left(&team);
    EXPECT_TRUE(list_equals(&team, NULL, 0));
}
