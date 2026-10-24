#ifndef VMNL_NET_LIST_TESTS_H
#define VMNL_NET_LIST_TESTS_H

#include "list/list_internal.h"

struct Player {
    struct VmnlNetListLink link;
    int id;
};

static inline bool check_list_links(const struct VmnlNetListLink *head, const int *ids, const size_t nids)
{
    const struct VmnlNetListLink *link = head;

    if (head == NULL) {
        return head->next == head && head->prev == head;
    }
    for (size_t it = 0; it < nids; it++) {
        if (link->next->prev != link || link->next == head || VMNL_NET_LIST_ENTRY(link->next, const struct Player, link)->id != ids[it]) {
            return false;
        }
        link = link->next;
    }
    return link->next == head && head->prev == link;
}

#endif /* !VMNL_NET_LIST_TESTS_H */
