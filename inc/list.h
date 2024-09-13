#ifndef   __s_list_h__
#define   __s_list_h__ 



struct list_head {	
	struct list_head  *prev, *next;
};


extern void INIT_HEAD(struct list_head *head);
int add_list(struct list_head *head, struct list_head *list);
void add_tail(struct list_head *head, struct list_head *list);

#endif
