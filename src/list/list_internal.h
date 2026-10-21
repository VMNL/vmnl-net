/**
 * \file list_internal.h
 * \brief Intrusive circular doubly linked list.
 *
 * A structure joins a list through the link it embeds.
 *
 * \par Pros
 * - O(1) insertion and removal, removal needing only the link.
 * - No allocation, so inserting cannot fail.
 * - A structure can join several lists, with one link per list.
 *
 * \par Cons
 * - O(n) traversal through scattered memory, slower than an array.
 * - vmnl_net_list_size() is O(n).
 * - No access by index.
 * - The list does not own its structures: remove them before freeing them.
 *
 * \par Example
 * \code
 * struct Client {
 *     struct VmnlNetListLink link;
 *     int id;
 * };
 *
 * struct VmnlNetListLink clients;
 * struct Client *client;
 *
 * struct Client alice = {.id = 1};
 * struct Client bob = {.id = 2};
 *
 * vmnl_net_list_init(&clients);                   // []
 * vmnl_net_list_push_back(&clients, &alice.link); // [alice]
 * vmnl_net_list_push_back(&clients, &bob.link);   // [alice, bob]
 * VMNL_NET_LIST_FOREACH(client, &clients, link) {
 *     printf("%d\n", client->id);                 // 1, then 2
 * }
 * vmnl_net_list_remove(&alice.link);              // [bob]
 * \endcode
 */

#ifndef VMNL_NET_LIST_INTERNAL_H
#define VMNL_NET_LIST_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Link of a list, embedded in the structure it links.
 * \remark The head of a list is a link to itself while the list is empty.
 */
struct VmnlNetListLink {
    struct VmnlNetListLink *prev; /*!< Previous link. */
    struct VmnlNetListLink *next; /*!< Next link. */
};

/**
 * \brief Gets the structure that embeds a link.
 * \param link Embedded link.
 * \param type Type of the structure.
 * \param member Name of the link in the structure.
 * \remark Getting the client from a link received alone:
 * \code
 * void greet_client(const struct VmnlNetListLink *client_link)
 * {
 *     // link is the name of the member in struct Client.
 *     const struct Client *client = VMNL_NET_LIST_ENTRY(client_link, const struct Client, link);
 *
 *     printf("Hi, client %d\n", client->id);
 * }
 * \endcode
 */
#define VMNL_NET_LIST_ENTRY(link, type, member) ((type *) (void *) ((char *) (link) - offsetof(type, member)))

/**
 * \brief Gets the first structure of a list.
 * \param head Head of the list.
 * \param type Type of the structure.
 * \param member Name of the link in the structure.
 * \attention \p head **MUST** not be empty: \p head itself would be converted, giving an invalid pointer.
 * \remark Checking for an empty list first:
 * \code
 * if (vmnl_net_list_is_empty(&clients) == false) {
 *     struct Client *first = VMNL_NET_LIST_FRONT(&clients, struct Client, link); // &alice
 *     struct Client *last = VMNL_NET_LIST_BACK(&clients, struct Client, link);   // &bob
 * }
 * \endcode
 */
#define VMNL_NET_LIST_FRONT(head, type, member) VMNL_NET_LIST_ENTRY((head)->next, type, member)

/**
 * \brief Gets the last structure of a list.
 * \param head Head of the list.
 * \param type Type of the structure.
 * \param member Name of the link in the structure.
 * \attention \p head **MUST** not be empty: \p head itself would be converted, giving an invalid pointer.
 * \remark See the example of #VMNL_NET_LIST_FRONT.
 */
#define VMNL_NET_LIST_BACK(head, type, member) VMNL_NET_LIST_ENTRY((head)->prev, type, member)

/**
 * \brief Iterates over the structures of a list.
 * \param it Pointer to the current structure.
 * \param head Head of the list.
 * \param member Name of the link in the structure.
 * \attention The \p it parameter **MUST** be an identifier, pasted into the cursor name `cursor_<it>`.
 * \attention Only the current structure can be removed: the cursor already holds the next one, which would loop on itself once removed.
 * \remark Removing the current structure:
 * \code
 * struct Client *client;
 *
 * VMNL_NET_LIST_FOREACH(client, &clients, link) {
 *     if (client->id == 2) {
 *         vmnl_net_list_remove(&client->link);
 *     }
 * }
 * \endcode
 */
#define VMNL_NET_LIST_FOREACH(it, head, member) \
    for (struct VmnlNetListLink *cursor_##it = (head)->next; cursor_##it != (head) && ((it) = VMNL_NET_LIST_ENTRY(cursor_##it, __typeof__(*(it)), member), cursor_##it = cursor_##it->next, true);)

/**
 * \brief Iterates over the structures of a list, backwards.
 * \param it Pointer to the current structure.
 * \param head Head of the list.
 * \param member Name of the link in the structure.
 * \attention The \p it parameter **MUST** be an identifier, pasted into the cursor name `cursor_<it>`.
 * \attention Only the current structure can be removed: the cursor already holds the previous one, which would loop on itself once removed.
 * \remark See the example of #VMNL_NET_LIST_FOREACH.
 */
#define VMNL_NET_LIST_FOREACH_REVERSE(it, head, member) \
    for (struct VmnlNetListLink *cursor_##it = (head)->prev; cursor_##it != (head) && ((it) = VMNL_NET_LIST_ENTRY(cursor_##it, __typeof__(*(it)), member), cursor_##it = cursor_##it->prev, true);)

/**
 * \brief Initializes a list, or a link outside any list.
 * \param head Head of the list, or a link.
 */
static inline void vmnl_net_list_init(struct VmnlNetListLink *head)
{
    head->prev = head;
    head->next = head;
}

/**
 * \brief Checks whether a list is empty.
 * \param head Head of the list.
 * \return true if the list holds no link, false otherwise.
 */
static inline bool vmnl_net_list_is_empty(const struct VmnlNetListLink *head)
{
    return head->next == head;
}

/**
 * \brief Counts the links of a list, in O(n).
 * \param head Head of the list.
 * \return Number of links, \p head excluded.
 */
static inline size_t vmnl_net_list_size(const struct VmnlNetListLink *head)
{
    size_t size = 0;

    for (const struct VmnlNetListLink *link = head->next; link != head; link = link->next) {
        size++;
    }
    return size;
}

/**
 * \brief Inserts a link after another one.
 * \param position Link to insert after, the head to insert at the front.
 * \param link Link to insert.
 * \attention \p link **MUST** not be in a list: its neighbours would keep pointing to it, corrupting that list.
 */
static inline void vmnl_net_list_insert_after(struct VmnlNetListLink *position, struct VmnlNetListLink *link)
{
    link->prev = position;
    link->next = position->next;
    position->next->prev = link;
    position->next = link;
}

/**
 * \brief Inserts a link before another one.
 * \param position Link to insert before, the head to insert at the end.
 * \param link Link to insert.
 * \attention \p link **MUST** not be in a list: its neighbours would keep pointing to it, corrupting that list.
 */
static inline void vmnl_net_list_insert_before(struct VmnlNetListLink *position, struct VmnlNetListLink *link)
{
    link->prev = position->prev;
    link->next = position;
    position->prev->next = link;
    position->prev = link;
}

/**
 * \brief Inserts a link at the front of a list.
 * \param head Head of the list.
 * \param link Link to insert.
 * \attention \p link **MUST** not be in a list: its neighbours would keep pointing to it, corrupting that list.
 */
static inline void vmnl_net_list_push_front(struct VmnlNetListLink *head, struct VmnlNetListLink *link)
{
    vmnl_net_list_insert_after(head, link);
}

/**
 * \brief Inserts a link at the end of a list.
 * \param head Head of the list.
 * \param link Link to insert.
 * \attention \p link **MUST** not be in a list: its neighbours would keep pointing to it, corrupting that list.
 */
static inline void vmnl_net_list_push_back(struct VmnlNetListLink *head, struct VmnlNetListLink *link)
{
    vmnl_net_list_insert_before(head, link);
}

/**
 * \brief Removes a link from its list.
 * \param link Link to remove.
 * \attention \p link **MUST** be in a list or initialized with vmnl_net_list_init(): its pointers are dereferenced.
 * \remark \p link then links to itself, like an empty list, so removing it again does nothing.
 */
static inline void vmnl_net_list_remove(struct VmnlNetListLink *link)
{
    link->prev->next = link->next;
    link->next->prev = link->prev;
    vmnl_net_list_init(link);
}

/**
 * \brief Merges a list to another.
 * \param head First list.
 * \param link List to merge.
 */
static inline void vmnl_net_list_merge(struct VmnlNetListLink *head, struct VmnlNetListLink *link)
{
    if (vmnl_net_list_is_empty(link) == true) {
        return;
    }
    head->prev->next = link->next;
    link->prev->next = head;
    vmnl_net_list_init(link);
}

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_LIST_INTERNAL_H */
