#pragma once

/* Linux kernel-like linked list. */

#include <stddef.h>		/* offsetof */

#define DSIO_LIST_ENTRY(PTR, TYPE, FIELD) \
	((TYPE *)(((char *)PTR) - offsetof(TYPE, FIELD)))

#define DSIO_LIST_INIT(list) {&list, &list}

#define dsio_list_foreach(ptr, head) \
	for (ptr = (head)->next; ptr != head; ptr = ptr->next)

#define dsio_list_foreach_rev(ptr, head) \
	for (ptr = (head)->prev; ptr != head; ptr = ptr->prev)

#define dsio_list_foreach_safe(ptr, head, tmp)	\
	for (ptr = (head)->next, tmp = ptr->next;	\
	     ptr != head;				\
	     ptr = tmp, tmp = tmp->next)

#define dsio_list_foreach_rev_safe(ptr, head, tmp)	\
	for (ptr = (head)->prev, tmp = ptr->prev;	\
	     ptr != head;				\
	     ptr = tmp, tmp = tmp->prev)

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
					struct dscore_list_head *list)
{
	item->next = list;
	item->prev = list->prev;
	list->prev->next = item;
	list->prev = item;
}

static inline void dscore_list_concat(struct dscore_list_head *item,
				      struct dscore_list_head *list)
{
	if (!dscore_list_is_empty(list)) {
		item->prev->next = list->next;
		list->next->prev = item->prev;
		item->prev = list->prev;
		list->prev->next = item;
	}
}
