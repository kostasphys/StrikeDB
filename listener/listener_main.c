#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <inc/messages.h>
#include <inc/listener/listener.h>
#include <inc/net/tcp.h>
#include <inc/IPC/mqueues.h>
#include <inc/debug.h>
#include <inc/atomic_ops.h>

#define __USE_POSIX
#include <signal.h>

int  listenerFd, newFd;
struct line_msg msg_buffer;
struct  skMqueue_Struct  sys_mqueues;
pthread_t   tid, authId;

//pthread_mutex_t listenPollMutex = PTHREAD_MUTEX_INITIALIZER;


struct connectThreads_t   connectThreads[connectionThreadsMax];
struct listenHash   socketHash[SocketArrayMax], liveConnections, *err;
struct listenHash  *connectHead,  *connectTail;
struct listenHash  *hashNode;

int main()
{
    struct listenHash   *err;
    char line[255];
    err = NULL;
    int ret;


    ThdMaskSignals();
    
    listenerStart();

    sprintf(line, "Listener is listening on Fd: %d \n", listenerFd);
    trace_file(line);
    
    int i;
    for(i=0; i < pollingThreads; i++)
    {
        if ( pthread_create(&tid, NULL, pollingThreadFn, NULL) < 0 )
        {
            printf("Error Initializing Polling  Threads \n");
            exit(-1);
        }
    }
    
   /*  if ( pthread_create(&authId, NULL, authThreadFn, NULL) < 0 )
    {
        printf("Error Initializing Authorization Thread \n");
        exit(-1);
    }
 */

    for(;;)
    {   

       sprintf(line, "Listening for new connections... \n");
       trace_file(line);

       /*We wait for connections here*/
       newFd = acceptConnect();
       if(newFd == -1)
       {    
            sprintf(line, "Error while accepting a new connection \n");
            trace_file(line);
       }

        int sockFlags;
        sockFlags = fcntl(newFd ,F_GETFL, 0);
        fcntl(newFd, F_SETFL, sockFlags | O_NONBLOCK);

       
        /*Authorize the connection*/
        ret = authorizeConn(newFd, 50);
        if(ret < 0 )
        {
            close(newFd);
            continue;
        }
            
        
        sprintf(line, "New connection accepted: %d \n", newFd);
        trace_file(line);


        hashNode = search_hash_node(newFd);

        sprintf(line, "After Search hash node: %d \n", newFd);
        trace_file(line);
        
        if(hashNode == NULL)
        {
            //hashNode = insert_hash_node(hashNode, newFd);
            //if(hashNode == NULL)
            //{
            /*We should print in the error log later on*/
                printf("Error when calling: insert_hash_node \n");
                close(newFd);
                break;
            //}
        }
        
        sprintf(line, "Before mutex lock: %d \n", newFd);
        trace_file(line);
        
        pthread_mutex_lock(&livePollMutex);

        atomic_inc_db(&hashNode->isAlive);
        
        sprintf(line, "Before HASHI \n");
        trace_file(line);
        add_item_hashLive(connectHead, hashNode);
        atomic_inc_db(&liveConnects);
        sprintf(line, "Before HASHI \n");
        trace_file(line);


        pthread_mutex_unlock(&livePollMutex);
        
        sprintf(line, "After lock: %d \n", newFd);
        trace_file(line);
        

        ret =  pthread_kill(tid, SIGUSR2);
        if(ret != 0 )
        {
            perror("Err: ");
            fprintf(stderr, "Error while sending SIGUSR2 signal to polling threads. We abort . %d\n", errno);
            exit(-1);
        }
        
    }
      
}