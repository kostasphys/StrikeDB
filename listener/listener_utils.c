#include <stdio.h>
#include <stdlib.h>
#include <inc/threads.h>
#include <inc/listener/listener.h>


unsigned int entry_hash_function(int fd)
{
    return ( fd ) % (SocketArrayMax );
}


struct listenHash *insert_hash_node(struct listenHash *socket, int fd)
{
    if( socket-> fd == -1)
    {
        socket->fd = fd;
        socket->next = NULL;
        return socket;
    }

    struct listenHash *ptr = malloc(sizeof(struct listenHash));
    if(ptr == NULL)
    {
        printf("Error during allocating memory for socket hash \n");
        return (struct listenHash *)NULL;
    }

    for(;;)
    {
        if(socket->next == NULL)
            break;
            
        socket = socket->next;
    }

    ptr->fd = fd;
    ptr->next = NULL;
    socket->next = ptr;
    return ptr;
}


struct listenHash *search_hash_node(int fd, struct listenHash  **err)
{
    unsigned int   hash;
    struct listenHash   *ptr; 

    *err = NULL;

    hash = entry_hash_function(fd);
    ptr = &socketHash[hash];

    for(;;)
    {
        if( ptr->fd == fd )
            return ptr;
        
        if(ptr->next != NULL)
            ptr = ptr->next;
        else
        {
            *err = ptr;
            return  (struct listenHash *)NULL;
        }
    }
}


void INIT_HEAD_MSG(struct connectThreadsInfo *head)
{
    head->next = head;
    head->prev = head;
}

void add_tail_msg(struct connectThreadsInfo  *head, struct connectThreadsInfo  *list )
{
    struct connectThreadsInfo *start = list->next;
    struct connectThreadsInfo *end = list->prev;
    struct connectThreadsInfo *head_prev = head->prev;
    
    head_prev->next = start;
    start->prev = head_prev;
    end->next = head;
    head->prev = end;
    
}