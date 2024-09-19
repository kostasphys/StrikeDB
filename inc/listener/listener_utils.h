#ifndef   __s_listener_utils_h__
#define   __s_listener_utils_h__


/********************************************************************************************/
unsigned int entry_hash_function(int fd);
struct listenHash *insert_hash_node(struct listenHash *socket, int fd);
struct listenHash *search_hash_node(int fd, struct listenHash  **err);
void INIT_HEAD_MSG(struct connectThreadsInfo *head);
void add_tail_msg(struct connectThreadsInfo  *head, struct connectThreadsInfo  *list);
/********************************************************************************************/
#endif