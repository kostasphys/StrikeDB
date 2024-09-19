#include <inc/list.h>
#include <stdio.h>

void INIT_HEAD(struct list_head *head)
{
	head->prev = head;
	head->next = head;
}

int add_list(struct list_head *head, struct list_head *list)
{
	return 0;
}


void add_tail(struct list_head *head, struct list_head *list){
	
	struct list_head *prev = head->prev;

	prev->next = list;
	list->prev = prev;
	list->next = head;
	head->prev = list;
}


/*Delete an element from the list*/
void del_list(struct list_head *list)
{
	list->prev->next = list->next;
	list->next->prev = list->prev;
}


