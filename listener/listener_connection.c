#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <inc/atomic_ops.h>
#include <inc/listener/listener.h>
#include <inc/list.h>
#include <inc/net/tcp.h>
#include <inc/debug.h>


void handle_connection(struct connectThreadsInfo  *msg, int arrIndex)
{
    struct listenHash   *hashNode, **err;
    int ret;
    char line[255];

    int readRetry = 0;
    
    

    sprintf(line, "Msg, fd:%d ThreadIndex:%d\n", msg->hashNode->fd, arrIndex);
    trace_file(line);

    
    
    /*In this loop we retry to read the socket a bunch of times before we wait for the next round*/
    readRetry  = 0;

    do
    {
        ret = readMsgFast(msg->fd, &msg->packet, (int *)&msg->rwBytes, 0);
        --readRetry;
    }
    while(readRetry > 0 && ret == -EAGAIN );
    
    
    sprintf(line, "Ret in  readMsgFast %d\n", ret);
    trace_file(line);

    switch(ret)
    {
        case 1: 
        {
            
            atomic_dec_db(&msg->hashNode->status);

            /*There is a posibily where we hit the case: -EAGAIN where we decriment it once. So we have to make sure we dont
              increment twice*/
            if(atomic_read_db(&msg->hashNode->readPoll) == 1)
                atomic_dec_db(&msg->hashNode->readPoll);
            

            generic_del_item(msg);

            add_item_conn(connectThreads[arrIndex].internalPool, msg );
        
            /*Send message to the processing threads*/
            sprintf(line, "SUCCESS: later we will change it: %s \n", msg->packet.buffer.buffer);
            trace_file(line);

            break;
        }
        case -EAGAIN:
        {

            if(atomic_read_db(&msg->hashNode->readPoll) == 1)
                atomic_dec_db(&msg->hashNode->readPoll);


            sprintf(line, "Request is blocked \n");
            trace_file(line);

            break;
        }
        case 0:
        {   
            generic_del_item(msg);

            add_item_conn(connectThreads[arrIndex].internalPool, msg );
        
            
            if(atomic_read_db(&msg->hashNode->isAlive) == 1)
                atomic_dec_db(&msg->hashNode->isAlive);

            atomic_dec_db(&msg->hashNode->status);

            sprintf(line, "The socket with fd %d is closed \n", msg->fd);
            trace_file(line);
            
            break;
        }
        default:
        {
            sprintf(line, "Default should not be allowed \n");
            trace_file(line);
            break;
        }
        
        
    }


}


void *thread_connection(void *arg)
{
    int arrIndex;
    struct connectThreadsInfo  *msgPtr;
    struct connectThreadsInfo  *msgHandle;
    char line[255];

    arrIndex = *(int *)arg;
    free(arg);

    while(1)
    {
        pthread_mutex_lock(&connectThreads[arrIndex].infoThreadMutex);
        while(connectThreads[arrIndex].requestFlag == 0)
            pthread_cond_wait(&connectThreads[arrIndex].infoThreadCond, &connectThreads[arrIndex].infoThreadMutex);
        
            
        connectThreads[arrIndex].requestFlag = 0;
        

        sprintf(line, "Thread is awake with number %d \n", arrIndex);
        trace_file(line);
        

        if( connectThreads[arrIndex].tempRequests->next == connectThreads[arrIndex].tempRequests )
            goto noTemp;
   

        add_tail_conn(connectThreads[arrIndex].pendingRequests, connectThreads[arrIndex].tempRequests);
        init_head_conn(connectThreads[arrIndex].tempRequests);

noTemp:
        pthread_mutex_unlock(&connectThreads[arrIndex].infoThreadMutex);

    
        /*We start at the end of the queue and process backwards. The reason for this is because the
          entries at the end most probably will be processed faster.
        */
        msgPtr = connectThreads[arrIndex].pendingRequests->prev;

        for(;;)
        {   

            if(msgPtr == connectThreads[arrIndex].pendingRequests )
            {
                break;
            }
                
            msgHandle = msgPtr;
            msgPtr = msgPtr->prev;

            handle_connection(msgHandle, arrIndex);
            
        }
    }
}

void init_temp_mempools(struct connectThreadsInfo  *head)
{

    int i;
    struct connectThreadsInfo  *ptr;


    for(i=0; i < tempMemPools; ++i)
    {
        ptr = malloc(sizeof(struct connectThreadsInfo));
        if(ptr == NULL)
        {
            fprintf(stderr, "Error when reserving memory for memPools \n");
            exit(0);
        }

        memset(ptr, '\0', sizeof(struct connectThreadsInfo));
        generic_add_list(head, ptr);
    }

}



int init_thread_connection()
{
    int i = 0;
    int *arg;
    pthread_t  tid;
    pthread_mutexattr_t mattr;
    pthread_condattr_t  cattr;
    struct connectThreadsInfo *ptrReq, *ptrTemp, *ptrPool, *ptrIntPool;
    
    pthread_mutexattr_init(&mattr);
	pthread_condattr_init(&cattr);


    for(i=0; i < connectionThreads; i++)
    {
        arg  = malloc(sizeof(int));
        if(arg == NULL)
        {
            printf("Error while allocating mem for connect args \n");
            exit(-1);
        }

        
        /*All those heads don't store any info. We just use them so that we can link all the necessary structures that we need*/
        ptrReq = malloc(sizeof(struct connectThreadsInfo));
        ptrTemp = malloc(sizeof(struct connectThreadsInfo));
        ptrPool = malloc(sizeof(struct connectThreadsInfo));
        ptrIntPool = malloc(sizeof(struct connectThreadsInfo));

        if(ptrReq == NULL || ptrTemp == NULL || ptrPool == NULL || ptrIntPool == NULL)
        {
            printf("Error when allocating header nodes for connectThreads \n");
            exit(-1);
        }

        init_head_conn(ptrReq);
        init_head_conn(ptrTemp);
        init_head_conn(ptrIntPool);
        init_head_conn(ptrPool);

        init_temp_mempools(ptrPool);

        ptrReq->fd = -1;
        ptrTemp->fd = -2;

        connectThreads[i].pendingRequests = ptrReq;
        connectThreads[i].tempRequests = ptrTemp;
        connectThreads[i].poolConns = ptrPool;
        connectThreads[i].internalPool = ptrIntPool;

        

        pthread_mutex_init( &(connectThreads[i].infoThreadMutex) , &mattr);
        pthread_cond_init ( &(connectThreads[i].infoThreadCond) , &cattr);
        
        connectThreads[i].requestFlag = 0;


        *arg = i;

        if ( pthread_create(&tid, NULL, thread_connection, arg) < 0 )
        {
            printf("Error Initializing connection thrreads   \n");
            exit(-1);
        }  


    }   

}