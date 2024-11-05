#ifndef   __s_listener_utils_h__
#define   __s_listener_utils_h__



/********************************************************************************************/
unsigned int entry_hash_function(int fd);
struct listenHash *insert_hash_node(struct listenHash *socket, int fd);
struct listenHash *search_hash_node(int fd);
void init_head_conn(struct connectThreadsInfo *head);
void init_hash_sock();
void add_tail_conn(struct connectThreadsInfo  *head, struct connectThreadsInfo  *list);
void delete_list_conn(struct connectThreadsInfo *element);
void del_item_hashLive(struct listenHash  *item);
void add_item_hashLive(struct listenHash  *header, struct listenHash  *item);
void debug_list_conn(struct connectThreadsInfo *head, char *str);
void add_item_conn(struct connectThreadsInfo  *head, struct connectThreadsInfo  *item );
/********************************************************************************************/
#endif