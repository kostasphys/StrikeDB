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

	printf("STring ok \n\n");
	
}

