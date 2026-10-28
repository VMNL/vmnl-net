#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_splice, moves_links_to_end)
{
    struct Player players[5] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}};
    struct VmnlNetListLink team;
    struct VmnlNetListLink bench;

    vmnl_net_list_init(&team);
    vmnl_net_list_init(&bench);
    for (size_t it = 0; it < 2; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    for (size_t it = 2; it < 5; it++) {
        vmnl_net_list_push_back(&bench, &players[it].link);
    }
    vmnl_net_list_splice(&team, &bench);
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 1, 2, 3, 4}, 5));
    EXPECT_TRUE(list_equals(&bench, NULL, 0));
}

UTEST(vmnl_net_list_splice, moves_links_into_empty_list)
{
    struct Player players[2] = {{.id = 0}, {.id = 1}};
    struct VmnlNetListLink team;
    struct VmnlNetListLink bench;

    vmnl_net_list_init(&team);
    vmnl_net_list_init(&bench);
    vmnl_net_list_push_back(&bench, &players[0].link);
    vmnl_net_list_push_back(&bench, &players[1].link);
    vmnl_net_list_splice(&team, &bench);
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 1}, 2));
    EXPECT_TRUE(list_equals(&bench, NULL, 0));
}

UTEST(vmnl_net_list_splice, moves_nothing_from_empty_list)
{
    struct Player players[2] = {{.id = 0}, {.id = 1}};
    struct VmnlNetListLink team;
    struct VmnlNetListLink bench;

    vmnl_net_list_init(&team);
    vmnl_net_list_init(&bench);
    vmnl_net_list_push_back(&team, &players[0].link);
    vmnl_net_list_push_back(&team, &players[1].link);
    vmnl_net_list_splice(&team, &bench);
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 1}, 2));
    EXPECT_TRUE(list_equals(&bench, NULL, 0));
}

UTEST(vmnl_net_list_splice, keeps_head_and_tail_linked)
{
    struct Player players[5] = {{.id = 0}, {.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}};
    struct VmnlNetListLink team;
    struct VmnlNetListLink bench;

    vmnl_net_list_init(&team);
    vmnl_net_list_init(&bench);
    vmnl_net_list_push_back(&team, &players[0].link);
    vmnl_net_list_push_back(&bench, &players[1].link);
    vmnl_net_list_push_back(&bench, &players[2].link);
    vmnl_net_list_splice(&team, &bench);
    vmnl_net_list_push_back(&team, &players[3].link);
    vmnl_net_list_push_front(&team, &players[4].link);
    EXPECT_TRUE(list_equals(&team, (int[]) {4, 0, 1, 2, 3}, 5));
}

UTEST(vmnl_net_list_splice, leaves_other_list_reusable)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;
    struct VmnlNetListLink bench;

    vmnl_net_list_init(&team);
    vmnl_net_list_init(&bench);
    vmnl_net_list_push_back(&bench, &players[0].link);
    vmnl_net_list_push_back(&bench, &players[1].link);
    vmnl_net_list_splice(&team, &bench);
    vmnl_net_list_push_back(&bench, &players[2].link);
    EXPECT_TRUE(list_equals(&bench, (int[]) {2}, 1));
    EXPECT_TRUE(list_equals(&team, (int[]) {0, 1}, 2));
}
