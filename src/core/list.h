/* Copyright (C) 2017 Andrew Mcdermott
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

#pragma once

/* Linux kernel-like linked list. */

#include <stddef.h>		/* offsetof */

#define DSIO_LIST_ENTRY(PTR, TYPE, FIELD) \
	((TYPE *)(((char *)PTR) - offsetof(TYPE, FIELD)))

#define DSIO_LIST_INIT(LIST) {&LIST, &LIST}

#define dsio_list_foreach(PTR, HEAD) \
	for (PTR = (HEAD)->next; PTR != HEAD; PTR = PTR->next)

#define dsio_list_foreach_rev(PTR, HEAD) \
	for (PTR = (HEAD)->prev; PTR != HEAD; PTR = PTR->prev)

#define dsio_list_foreach_safe(PTR, HEAD, TMP)		\
	for (PTR = (HEAD)->next, TMP = PTR->next;	\
	     PTR != HEAD;				\
	     PTR = TMP, TMP = TMP->next)

#define dsio_list_foreach_rev_safe(PTR, HEAD, TMP)	\
	for (PTR = (HEAD)->prev, TMP = PTR->prev;	\
	     PTR != HEAD;				\
	     PTR = TMP, TMP = TMP->prev)

struct dsio_list_head {
	struct dsio_list_head *next;
	struct dsio_list_head *prev;
};

static inline void dsio_list_init(struct dsio_list_head *head)
{
	head->next = head;
	head->prev = head;
}

static inline int dsio_list_is_empty(struct dsio_list_head *list)
{
	return list->next == list && list->prev == list;
}

static inline void dsio_list_remove(struct dsio_list_head *item)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;
}

static inline void dsio_list_add_first(struct dsio_list_head *item,
				       struct dsio_list_head *list)
{
	item->next = list->next;
	item->prev = list;
	list->next->prev = item;
	list->next = item;
}

static inline void dsio_list_add_last(struct dsio_list_head *item,
				      struct dsio_list_head *list)
{
	item->next = list;
	item->prev = list->prev;
	list->prev->next = item;
	list->prev = item;
}

static inline void dsio_list_concat(struct dsio_list_head *item,
				    struct dsio_list_head *list)
{
	if (!dsio_list_is_empty(list)) {
		item->prev->next = list->next;
		list->next->prev = item->prev;
		item->prev = list->prev;
		list->prev->next = item;
	}
}
