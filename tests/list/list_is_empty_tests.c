#include "list_tests.h"

#include "utest.h"

UTEST(vmnl_net_list_is_empty, returns_true_when_new)
{
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    EXPECT_TRUE(vmnl_net_list_is_empty(&team));
}

UTEST(vmnl_net_list_is_empty, returns_false_with_one_link)
{
    struct Player player = {.id = 0};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    vmnl_net_list_push_back(&team, &player.link);
    EXPECT_FALSE(vmnl_net_list_is_empty(&team));
}

UTEST(vmnl_net_list_is_empty, returns_false_with_several_links)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    EXPECT_FALSE(vmnl_net_list_is_empty(&team));
}

UTEST(vmnl_net_list_is_empty, returns_true_after_removing_every_link)
{
    struct Player players[3] = {{.id = 0}, {.id = 1}, {.id = 2}};
    struct VmnlNetListLink team;

    vmnl_net_list_init(&team);
    for (size_t it = 0; it < 3; it++) {
        vmnl_net_list_push_back(&team, &players[it].link);
    }
    for (size_t it = 0; it < 3; it++) {
        EXPECT_FALSE(vmnl_net_list_is_empty(&team));
        vmnl_net_list_remove(&players[it].link);
    }
    EXPECT_TRUE(vmnl_net_list_is_empty(&team));
}
