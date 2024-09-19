#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <inc/atomic_ops.h>
#include <inc/listener/listener.h>
#include <inc/listener/listener_utils.h>


int handle_connection(struct connectThreadsInfo  *msg)
{
    struct listenHash   *hashNode, **err;

    /*
    hashNode = search_hash_node(msg->fd, err);

    if(hashNode == NULL)    
    {
       hashNode = insert_hash_node(*err, msg->fd);
       if(hashNode == NULL)
            return -1;
    }
    */
    atomic_inc_db(&msg->isBlocked);  

    if(!atomic_read_db(&msg->hashNode->status))  
        atomic_inc_db(&msg->hashNode->status);



}


void *thread_connection(void *arg)
{
    int arrIndex;
    struct connectThreadsInfo  *msgPtr;

    arrIndex = *(int *)arg;
    free(arg);

    while(1)
    {
        pthread_mutex_lock(&connectThreads[arrIndex].infoThreadMutex);
        while(&connectThreads[arrIndex].requestFlag == 0);
            pthread_cond_wait(&connectThreads[arrIndex].infoThreadCond, &connectThreads[arrIndex].infoThreadMutex);
        
        pthread_mutex_lock(&connectThreads[arrIndex].infoThreadMutex);

        connectThreads[arrIndex].requestFlag = 0;

        add_tail_msg(connectThreads[arrIndex].pendingRequests, connectThreads[arrIndex].tempRequests);
        INIT_HEAD_MSG(connectThreads[arrIndex].tempRequests);

        pthread_mutex_unlock(&connectThreads[arrIndex].infoThreadMutex);

        /*We start at the end of the queue and process backwards. The reason for this is because the
          entries at the end most probably will be processed faster.
        */
        msgPtr = connectThreads[arrIndex].pendingRequests->prev;
        for(;;)
        {   
            
            handle_connection(msgPtr);

            msgPtr = msgPtr->next;
            if(msgPtr == connectThreads[arrIndex].pendingRequests )
            {
                break;
            }
        }
    }
}



int init_thread_connection()
{
    int i = 0;
    pthread_t  tid;
    pthread_mutexattr_t mattr;
    pthread_condattr_t  cattr;
    struct connectThreadsInfo *ptrReq, *ptrTemp;
    
    for(i=0; i < connectionThreads; i++)
    {
        int *arg  = malloc(sizeof(int));
        if(arg == NULL)
        {
            printf("Error while allocating mem for connect args \n");
            exit(-1);
        }

        *arg = i;

        if ( pthread_create(&tid, NULL, thread_connection, arg) < 0 )
        {
            printf("Error Initializing connection thrreads   \n");
            exit(-1);
        }  

        ptrReq = malloc(sizeof(struct connectThreadsInfo));
        ptrTemp = malloc(sizeof(struct connectThreadsInfo));

        if(ptrReq == NULL || ptrTemp == NULL)
        {
            printf("Error when allocating header nodes for connectThreads \n");
            exit(-1);
        }

        INIT_HEAD_MSG(ptrReq);
        INIT_HEAD_MSG(ptrTemp);

        connectThreads[i].pendingRequests = ptrReq;
        connectThreads[i].tempRequests = ptrTemp;

        pthread_mutex_init( &(connectThreads[i].infoThreadMutex) , &mattr);
        pthread_cond_init ( &(connectThreads[i].infoThreadCond) , &cattr);
        
        connectThreads[i].requestFlag = 0;
    }   

}