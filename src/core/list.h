#pragma once

/*
 * Copyright 2017 Andrew McDermott
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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