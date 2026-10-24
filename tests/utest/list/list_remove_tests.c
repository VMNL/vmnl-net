#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_remove, removes_first_link)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[0].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {1, 2}, 2));
}

UTEST(vmnl_net_list_remove, removes_middle_link)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[1].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {0, 2}, 2));
}

UTEST(vmnl_net_list_remove, removes_last_link)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[2].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {0, 1}, 2));
}

UTEST(vmnl_net_list_remove, removes_from_single_element_list)
{
    struct Player player = {.id = 0};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_back(&team, &player.link);
    vmnl_net_list_remove(&player.link);
    EXPECT_TRUE(check_list_links(&team, NULL, 0));
}

UTEST(vmnl_net_list_remove, removes_links_in_any_order)
{
    struct Player players[5] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 5; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[3].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {0, 1, 2, 4}, 4));
    vmnl_net_list_remove(&players[0].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {1, 2, 4}, 3));
    vmnl_net_list_remove(&players[4].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {1, 2}, 2));
    vmnl_net_list_push_back(&team, &players[0].link);
    vmnl_net_list_push_front(&team, &players[3].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {3, 1, 2, 0}, 4));
    vmnl_net_list_remove(&players[1].link);
    vmnl_net_list_remove(&players[2].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {3, 0}, 2));
    vmnl_net_list_remove(&players[3].link);
    vmnl_net_list_remove(&players[0].link);
    EXPECT_TRUE(check_list_links(&team, NULL, 0));
}

UTEST(vmnl_net_list_remove, removed_link_links_to_itself)
{
    struct Player players[2] = {{.id = 0}, {.id = 1}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_back(&team, &players[0].link);
    vmnl_net_list_push_back(&team, &players[1].link);
    vmnl_net_list_remove(&players[0].link);
    EXPECT_EQ(players[0].link.next, &players[0].link);
    EXPECT_EQ(players[0].link.prev, &players[0].link);
}

UTEST(vmnl_net_list_remove, ignores_second_removal)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    vmnl_net_list_remove(&players[1].link);
    vmnl_net_list_remove(&players[1].link);
    EXPECT_TRUE(check_list_links(&team, (int[]) {0, 2}, 2));
}
