#ifndef   __s_list_h__
#define   __s_list_h__ 

#include <inc/types/type_defs.h>
#include <stdio.h>

#define GEN_INIT_HEAD(header)({ \
	header->next = header; 	\
	header->prev = header;	\
})


#define generic_add_list(header, item)({ \
	vartype(header)	prev; \
	prev = header->prev; \
							\
	prev->next = item;	 \
	item->prev = prev;	\
	item->next = header;	\
	header->prev = item;	\
})


#define generic_del_item(item)({ \
	vartype(item)	prev, next; \
	prev = item->prev; \
	next = item->next; \
					\
	prev->next = next; \
	next->prev = prev; \
})



struct list_head {	
	struct list_head  *prev, *next;
};



extern void INIT_HEAD(struct list_head *head);
int add_list(struct list_head *head, struct list_head *list);
void add_tail(struct list_head *head, struct list_head *list);

#endif
