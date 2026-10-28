#include "list_tests.h"

#include "utest.h"

struct Score {
    int points;
    struct VmnlNetListLink link;
    int bonus;
};

UTEST(VMNL_NET_LIST_ENTRY, returns_structure_of_each_link)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    EXPECT_EQ(VMNL_NET_LIST_ENTRY(team.next, struct Player, link), &players[0]);
    EXPECT_EQ(VMNL_NET_LIST_ENTRY(team.next->next, struct Player, link), &players[1]);
    EXPECT_EQ(VMNL_NET_LIST_ENTRY(team.prev, struct Player, link), &players[2]);
}

UTEST(VMNL_NET_LIST_ENTRY, returns_structure_with_link_in_middle)
{
    struct Score scores[2] = {{.points = 10, .bonus = 1}, {.points = 20, .bonus = 2}};
    struct VmnlNetListLink board;

    vmnl_net_list_init(&board);
    vmnl_net_list_push_back(&board, &scores[0].link);
    vmnl_net_list_push_back(&board, &scores[1].link);
    EXPECT_EQ(VMNL_NET_LIST_ENTRY(board.next, struct Score, link), &scores[0]);
    EXPECT_EQ(VMNL_NET_LIST_ENTRY(board.prev, struct Score, link)->points, 20);
    EXPECT_EQ(VMNL_NET_LIST_ENTRY(board.prev, struct Score, link)->bonus, 2);
}

UTEST(VMNL_NET_LIST_ENTRY, returns_const_structure)
{
    const struct Player player = {.id = 7};

    EXPECT_EQ(VMNL_NET_LIST_ENTRY(&player.link, const struct Player, link)->id, 7);
}
