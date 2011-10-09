/*
 * Copyright (C) 2010 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIST_H
#define __LIST_H

#include "macro.h"

/**
 * @brief Declares a double linked list head.
 * @param name Name of the list head structure to generate.
 * @param type Structure name of an item.
 * @return Generated structure for a linked list head.
 */

/* TODO: why do we need name here, while not for LIST_ENTRY()? */
#define LIST_HEAD(name, type) \
    struct name \
    { \
	struct type *first; \
	struct type *last;  \
    }

/**
 * @brief Declares a linked list item entry.
 * @param type Structure name of an item.
 * @return Generated structure for a linked list item entry.
 */
#define LIST_ENTRY(type)   \
    struct                 \
    {                      \
	struct type *prev; \
	struct type *next; \
    }

/**
 * @brief Determine whether the list is empty.
 * @param head Pointer to the head of the list.
 * @return Non-zero if empty, and zero otherwise.
 */
#define LIST_EMPTY(head) \
    ((head)->first == NULL && (head)->last == NULL)

/**
 * @brief Retrieve the first element on the list.
 * @param head Pointer to the head of the list.
 * @return Pointer to the first element on the list, if any.
 */
#define LIST_FIRST(head) \
    (head)->first

/**
 * @brief Retrieve the last element on the list.
 * @param head Pointer to the head of the list.
 * @return Pointer to the last element on the list, if any.
 */
#define LIST_LAST(head) \
    (head)->last

/**
 * @brief Retrieve the next element given an item.
 * @param item Item pointer.
 * @param field Name of the linked list entry field on the item.
 * @return Pointer to the next element, if any.
 */
#define LIST_NEXT(item, field) \
    (item)->field.next

/**
 * @brief Retrieve the previous element given an item.
 * @param item Item pointer.
 * @param field Name of the linked list entry field on the item.
 * @return Pointer to the previous element, if any.
 */
#define LIST_PREV(item, field) \
    (item)->field.prev

/**
 * @brief Initialize a list head.
 * @param head Pointer to the head of a linked list.
 */
#define LIST_INIT(head)  \
    (head)->first = NULL; \
    (head)->last  = NULL;

/**
 * @brief Insert a new item at the beginning of the list.
 * @param head Pointer to the head of the list.
 * @param item Pointer to the item to be inserted.
 * @param field Name of the linked list entry field on the item.
 */
#define LIST_INSERT_HEAD(head, item, field) \
    if ((head)->first != NULL)              \
    {                                       \
	(item)->field.next = (head)->first; \
	(item)->field.prev = NULL;          \
	(head)->first->field.prev = (item); \
	(head)->first = (item);             \
    }                                       \
    else                                    \
    {                                       \
	(head)->first = (item);             \
	(head)->last  = (item);             \
	(item)->field.next = NULL;          \
	(item)->field.prev = NULL;          \
    }

/**
 * @brief Insert a new item at the end of the list.
 * @param head Pointer to the head of the list.
 * @param item Pointer to the item to be inserted.
 * @param field Name of the linked list entry field on the item.
 */
#define LIST_INSERT_TAIL(head, item, field) \
    if ((head)->last != NULL)               \
    {                                       \
	(item)->field.next = NULL;          \
	(item)->field.prev = (head)->last;  \
	(head)->last->field.next = (item);  \
	(head)->last = (item);              \
    }                                       \
    else                                    \
    {                                       \
	(head)->first = (item);             \
	(head)->last  = (item);             \
	(item)->field.next  = NULL;         \
	(item)->field.prev  = NULL;         \
    }

/**
 * @brief Remove an item from the list.
 * @param head Pointer to the head of the list.
 * @param item The item to remove.
 * @param field Name of the linked list entry field on the item.
 */
#define LIST_REMOVE(head, item, field)                       \
    if ((item)->field.prev == NULL)                          \
	(head)->first = (item)->field.next;                  \
    else                                                     \
	(item)->field.prev->field.next = (item)->field.next; \
                                                             \
    if ((item)->field.next == NULL)                          \
	(head)->last = (item)->field.prev;                   \
    else                                                     \
	(item)->field.next->field.prev = (item)->field.prev;

/**
 * Loop a linked list.
 * @param var Variable for storing the item pointer.
 * @param head Pointer to the head of the list.
 * @param Name of the linked list entry field on the item.
 */
#define LIST_FOREACH(var, head, field)         \
    for ((var) = (head)->first; (var) != NULL; \
	 (var) = (var)->field.next)

/**
 * Loop a linked list while allowing removing elements.
 * @param var Variable for storing the item pointer.
 * @param head Pointer to the head of the list.
 * @param Name of the linked list entry field on the item.
 * @param tmpvar Temporary variable for storing an item pointer.
 */
#define LIST_FOREACH_SAFE(var, head, field, tmpvar)            \
    for ((var) = (head)->first;                                \
         (var) != NULL && (((tmpvar) = (var)->field.next), 1); \
         (var) = (tmpvar))

#endif /* __LIST_H */
