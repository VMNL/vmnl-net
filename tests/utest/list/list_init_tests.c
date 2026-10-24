#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_init, links_head_to_itself)
{
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    EXPECT_EQ(team.next, &team);
    EXPECT_EQ(team.prev, &team);
}

UTEST(vmnl_net_list_init, empties_list)
{
    struct Player players[2] = {{.id = 0}, {.id = 1}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_back(&team, &players[0].link);
    vmnl_net_list_push_back(&team, &players[1].link);
    vmnl_net_list_init(&team);
    EXPECT_TRUE(check_list_links(&team, NULL, 0));
}

UTEST(vmnl_net_list_init, prepares_link_for_removal)
{
    struct Player player = {.id = 0};

    vmnl_net_list_init(&player.link);
    vmnl_net_list_remove(&player.link);
    EXPECT_EQ(player.link.next, &player.link);
    EXPECT_EQ(player.link.prev, &player.link);
}
