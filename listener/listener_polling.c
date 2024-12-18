#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <inc/net/tcp.h>
#include <inc/debug.h>
#include <inc/atomic_ops.h>
#include <inc/listener/listener_utils.h>




#define max_val(x,y) (((x) >= (y)) ? (x) : (y))
#define min_val(x,y) (((x) >= (y)) ? (y) : (x))


int maxFd = 0;
int indexFd, threadIndex, tempThreadIndex;
char line[255];

pthread_mutex_t livePollMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  livePollCond  = PTHREAD_COND_INITIALIZER;


int liveConnects = 0;
int connectCounter = 0;
struct listenHash   *TailPtr;



void checkLiveConn()
{   
    struct listenHash   *hashPtr;

    if(atomic_read_db(&liveConnects) == 0)
        return;

    sprintf(line, "Before locking\n");
    trace_file(line);


    pthread_mutex_lock(&livePollMutex);

    TailPtr = connectHead->livePrev;

    connectCounter += liveConnects;
    liveConnects = 0;

    
    pthread_mutex_unlock(&livePollMutex);

    hashPtr = connectHead;

    do
    {   
        hashPtr = hashPtr->liveNext;

        sprintf(line, "Alive sockets %d\n", hashPtr->fd);
        trace_file(line);

        maxFd = max_val(maxFd, hashPtr->fd);


    } 
    while (hashPtr != TailPtr);
    
    
    sprintf(line, "End of connect\n");
    trace_file(line);

    return;
}


void *pollingThreadFn(void *arg)
{
    int   ret;
    fd_set  readfds;
    sigset_t   set;
    
    

    struct timeval timerSelect;

    
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);



    /*We need TailPtr so we can determine the last element of the Connection Alive array before we sleep.
      During sleeping the listening thread might add some new connections on the queue. Because of this we must
      find the maximum fd so that we don't make pointless itterations  */
    TailPtr = connectHead;



    while(1)
    {   
     //   struct line_packet   msg;
        struct listenHash   *hashPtr, *hashPrev, *hashSearch, *hashTemp;
        struct connectThreadsInfo   *cnnPtr;
        
checkLive:
        checkLiveConn();


        if( connectCounter == 0 )
        {   
            if(atomic_read_db(&liveConnects) == 0)
                goto checkLive;
        	
            sprintf(line, "There are no live connections... we sleep \n");
            trace_file(line);
            
            suspend_thread(&set);
            //We have to calibrate everything again
            goto checkLive;
        
        }

        

        FD_ZERO(&readfds);


        hashPtr = connectHead;
/* 
        if(hashPtr->liveNext == connectHead)
            goto select; */


        while( hashPtr != TailPtr )
        {   
            hashPtr = hashPtr->liveNext;

          
            if( atomic_read_db(&hashPtr->isAlive) == 0)
            {
                min_val(maxFd, hashPtr->fd);

                sprintf(line, "General fault: We break the connection \n");
                trace_file(line);

                pthread_mutex_lock(&livePollMutex);

                shutdown(hashPtr->fd ,SHUT_RDWR);
                if (close(hashPtr->fd) == -1) {
                    sprintf(line, " ERROR closing the socket %d \n", errno);
                    trace_file(line);
	            }

                --connectCounter;

                if( hashPtr == TailPtr )
                {
                    TailPtr = hashPtr->livePrev;

                    sprintf(line, " IT its tail \n");
                    trace_file(line);

                    debug_hashLive();
                    del_item_hashLive(hashPtr);
                    debug_hashLive();
                    pthread_mutex_unlock(&livePollMutex);    

                    
                    break;
                }
                

                hashTemp = hashPtr;
                hashPtr = hashPtr->livePrev;

                sprintf(line, " Usual case\n");
                trace_file(line);


                debug_hashLive();
                del_item_hashLive(hashTemp);
                debug_hashLive();

                pthread_mutex_unlock(&livePollMutex);

                /* if(hashPtr == connectHead)
                    break; */
                    
                continue;
            }

            FD_SET(hashPtr->fd, &readfds);
        }

 select:        
        timerSelect.tv_sec = 5;
        timerSelect.tv_usec = 0;

        sprintf(line, "Waiting on select with maxFd: %d\n\n", maxFd);
        trace_file(line);

        
        ret = select(maxFd + 1, &readfds, NULL, NULL, &timerSelect);

        if ( ret == 0 )
        {
            sprintf(line, "Time-out Occured \n");
            trace_file(line);
        }
        else if (ret < 0)
        {
            sprintf(line, "Error during select, ret: %d , errno: %d\n",ret ,errno);
            trace_file(line);
        }


        sprintf(line, "Some Descriptors are ready for reading\n");
        trace_file(line);

        /*Inform the threads about new activity in alive sockets*/
        for(indexFd=0; indexFd <= maxFd; ++indexFd)
        {   
            
            if ( !FD_ISSET(indexFd, &readfds) )
                continue;


            /*Find the hash node that corresponds to the indexFd file descriptors. If there isn't one we create it*/
            hashSearch = search_hash_node(indexFd);
            if( hashSearch == NULL)
            {
                sprintf(line, "Error when searching for hash node in Polling thread\n");
                trace_file(line);
            }

            
                

            /*If the connection is blocked we inform the connect thread that is responsible for 
             this connection and then we continue. */
             
            if( atomic_read_db(&hashSearch->status) == 1 )
            {   
                
                if(hashSearch->readPoll == 1)
                {       
                    //sprintf(line, "SKIP POLLING %d\n", hashSearch->threadArrIndex);
                    //trace_file(line);
                    continue;
                }
                

                tempThreadIndex = hashSearch->threadArrIndex;

                sprintf(line, "Polling block-waiting %d\n", tempThreadIndex);
                trace_file(line);

                pthread_mutex_lock(&connectThreads[tempThreadIndex].infoThreadMutex);
                connectThreads[tempThreadIndex].requestFlag = 1;
                pthread_cond_signal(&connectThreads[tempThreadIndex].infoThreadCond);
                pthread_mutex_unlock(&connectThreads[tempThreadIndex].infoThreadMutex);
                
                continue;
            }
                

            pthread_mutex_lock(&connectThreads[threadIndex].infoThreadMutex);


            cnnPtr = connectThreads[threadIndex].poolConns->next;


            if ( cnnPtr == connectThreads[threadIndex].poolConns )
                cnnPtr = malloc(sizeof(struct connectThreadsInfo));
            else    
                delete_list_conn(cnnPtr);

            memset(cnnPtr, '\0', sizeof(struct connectThreadsInfo));
            cnnPtr->fd = indexFd;
            cnnPtr->hashNode = hashSearch;
            cnnPtr->hashNode->threadArrIndex = threadIndex;
                
        
            generic_add_list(connectThreads[threadIndex].tempRequests, cnnPtr);


            atomic_inc_db(&hashSearch->status);
            atomic_inc_db(&hashSearch->readPoll);

            connectThreads[threadIndex].requestFlag = 1;

            pthread_cond_signal(&connectThreads[threadIndex].infoThreadCond);

            pthread_mutex_unlock(&connectThreads[threadIndex].infoThreadMutex);

    
    
            threadIndex = (threadIndex + 1)%(connectionThreads);

        }

     
    }
}