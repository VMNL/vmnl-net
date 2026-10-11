/**
 * \file list_internal.h
 * \brief Intrusive circular doubly linked list.
 * \note Port of the intrusive list of the Linux kernel (<linux/list.h>), whose design belongs to its authors.
 */

#ifndef VMNL_NET_LIST_INTERNAL_H
#define VMNL_NET_LIST_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Node of a list, embedded in the structure it links.
 * \note The head of a list is a node that links to itself while the list is empty.
 */
typedef struct VmnlNetList {
    struct VmnlNetList *prev; /*!< Previous node. */
    struct VmnlNetList *next; /*!< Next node. */
} VmnlNetList;

/**
 * \brief Gets the structure that embeds a node.
 * \param node Embedded node.
 * \param type Type of the structure.
 * \param member Name of the node in the structure.
 */
#define VMNL_NET_LIST_ENTRY(node, type, member) ((type *) (void *) ((char *) (node) - offsetof(type, member)))

/**
 * \brief Makes an empty list.
 * \param head Head of the list.
 */
static inline void vmnl_net_list_init(VmnlNetList *head)
{
    head->prev = head;
    head->next = head;
}

/**
 * \brief Inserts a node before another one.
 * \param node Node to insert, **MUST** not be in a list.
 * \param position Node to insert before, the head to insert at the end.
 */
static inline void vmnl_net_list_insert(VmnlNetList *node, VmnlNetList *position)
{
    node->next = position;
    node->prev = position->prev;
    position->prev->next = node;
    position->prev = node;
}

/**
 * \brief Removes a node from its list.
 * \param node Node to remove.
 * \note The node then links to itself, like an empty list, so removing it again does nothing.
 */
static inline void vmnl_net_list_remove(VmnlNetList *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    vmnl_net_list_init(node);
}

/**
 * \brief Checks whether a list is empty.
 * \param head Head of the list.
 * \return true if the list holds no node, false otherwise.
 */
static inline bool vmnl_net_list_empty(const VmnlNetList *head)
{
    return head->next == head;
}

#ifdef __cplusplus
}
#endif

#endif /* !VMNL_NET_LIST_INTERNAL_H */
