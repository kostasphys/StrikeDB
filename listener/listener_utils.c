#include <stdio.h>
#include <stdlib.h>
#include <inc/threads.h>
#include <inc/listener/listener.h>
#include <inc/types/type_defs.h>


unsigned int entry_hash_function(int fd)
{
    return ( fd ) % (SocketArrayMax );
}

void init_hash_sock()
{   int i;
    
    connectHead = malloc(sizeof(struct listenHash));
    if(connectHead == NULL)
    {
        fprintf(stderr, "Can't allocate connectHead \n");
        exit(0);
    }

    connectHead->fd = -1;
    connectHead->next = NULL;
    connectHead->liveNext = connectHead;
    connectHead->livePrev = connectHead;


    for(i=0; i<SocketArrayMax; i++)
    {
        socketHash[i].fd = i;
        socketHash[i].next = NULL;
        socketHash[i].liveNext = NULL;
        socketHash[i].isAlive = 0;
        socketHash[i].status = 0;
        socketHash[i].readPoll = 0;
    }

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
    smp_mb();
    socket->next = ptr;
    return ptr;
}


struct listenHash *search_hash_node(int fd)
{
    unsigned int   hash;
    struct listenHash   *ptr; 


    hash = entry_hash_function(fd);
    ptr = &socketHash[hash];

    for(;;)
    {
        if( ptr->fd == fd )
            return ptr;
        
        /*Here we are sure that the ptr->next will be in a consistent state.*/
        if(ptr->next != NULL)
            ptr = ptr->next;
        else
            return  insert_hash_node(ptr, fd);   
    }
}


void init_head_conn(struct connectThreadsInfo *head)
{
    head->next = head;
    head->prev = head;
}

void add_tail_conn(struct connectThreadsInfo  *head, struct connectThreadsInfo  *list )
{
    struct connectThreadsInfo *start = list->next;
    struct connectThreadsInfo *end = list->prev;
    struct connectThreadsInfo *head_prev = head->prev;
    
    head_prev->next = start;
    start->prev = head_prev;
    end->next = head;
    head->prev = end;
    
}


void add_item_conn(struct connectThreadsInfo  *head, struct connectThreadsInfo  *item )
{
    struct connectThreadsInfo *head_prev = head->prev;

    head_prev->next = item;
    item->prev = head_prev;
    item->next = head;
    head->prev = item;
    
}



void add_item_hashLive(struct listenHash  *header, struct listenHash  *item)
{
    struct listenHash  *prev; 
	prev = header->livePrev; 
							
	prev->liveNext = item;	 
	item->livePrev = prev;	
	item->liveNext = header;	
	header->livePrev = item;	
}


void del_item_hashLive(struct listenHash  *item)
{
    struct listenHash  *livePrev = item->livePrev;
    struct listenHash  *liveNext = item->liveNext;
    
    livePrev->liveNext = item->liveNext;
    liveNext->livePrev = item->livePrev;

    item->liveNext = NULL;
    item->livePrev = NULL;
}


void add_tail_hashlive(struct listenHash  *head, struct listenHash  *list )
{
    struct listenHash *start = list->liveNext;
    struct listenHash *end = list->livePrev;
    struct listenHash *head_prev = head->livePrev;
    
    head_prev->liveNext = start;
    start->livePrev = head_prev;
    end->liveNext = head;
    head->livePrev = end;
    
}



void delete_list_conn(struct connectThreadsInfo *element)
{
    struct connectThreadsInfo *prev = element->prev;
    struct connectThreadsInfo *next = element->next;

    prev->next = element->next;
    next->prev = prev;
}

void debug_list_conn(struct connectThreadsInfo *head, char *str)
{
     int ii = 0;
     char line[255];

     struct connectThreadsInfo *ptr;

    ptr = head;

     for(;;)
        {
            sprintf(line, " DEBUG %s: %d, fd: %d, ptr: %p\n",str ,ii ,ptr->fd, ptr);
            trace_file(line);    

            ptr = ptr->next;

            if(ptr == head)
            {
                sprintf(line, "We reached the end !!!!\n");
                trace_file(line);    
                break;
            }

            ++ii;
        }

}

void add_live_connection(struct listenHash * ptr)
{
    return;
}

