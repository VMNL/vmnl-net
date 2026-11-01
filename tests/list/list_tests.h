/**
 * \file list_tests.h
 * \brief Structure and helper of the list tests.
 */

#ifndef VMNL_NET_LIST_TESTS_H
#define VMNL_NET_LIST_TESTS_H

#include "list/list_internal.h"

/**
 * \brief Structure linked in the lists of the tests.
 */
struct Player {
    struct VmnlNetListLink link; /*!< Link in a list. */
    int id; /*!< Identifier checked by list_equals(). */
};

/**
 * \brief Checks the players of a list.
 * \param head Head of the list.
 * \param ids Expected identifiers, in order.
 * \param nids Number of elements of \p ids.
 * \return true if \p head links the players of \p ids in order, forwards and backwards, false otherwise.
 */
static inline bool list_equals(const struct VmnlNetListLink *head, const int *ids, size_t nids)
{
    const struct VmnlNetListLink *link = head;

    for (size_t it = 0; it < nids; it++) {
        if (link->next->prev != link || link->next == head || VMNL_NET_LIST_ENTRY(link->next, const struct Player, link)->id != ids[it]) {
            return false;
        }
        link = link->next;
    }
    return link->next == head && head->prev == link;
}

#endif /* !VMNL_NET_LIST_TESTS_H */
