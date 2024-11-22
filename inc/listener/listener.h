#ifndef   __s_listener_h__
#define   __s_listener_h__

#include <inc/list.h>
#include <inc/threads.h>
#include <inc/debug.h>
#include <sys/types.h>
#include <inc/listener/listener_threads.h>
#include <inc/atomic_ops.h>
#include <inc/messages.h>
#include <inc/net/tcp.h>

#define  __USE_POSIX
#define  __USE_XOPEN_EXTENDED

#define listenerPort 7000
#define AUTH_MAGIC_NUMBER "FFEEDD00"

struct connectThreadsInfo
{   
    int fd;
    pthread_t tid;

    struct connectThreadsInfo   *prev, *next;
    /* If we cant read the whole message in one  trial then we store the already bytes here */
    int       isBlocked;
    struct line_packet    packet;
    ssize_t     rwBytes;
    size_t      msgSize;
    /*This is for the listener thread. We need an efficient way to communicate the status of the socket*/
    struct listenHash   *hashNode;    

};

struct connectThreads_t
{
  //struct connectThreadsInfo SocketArr[SocketArrayMax];
  struct connectThreadsInfo  *pendingRequests;
  struct connectThreadsInfo  *tempRequests;
  struct connectThreadsInfo  *poolConns;
  struct connectThreadsInfo  *internalPool;
  pthread_mutex_t infoThreadMutex ; 
  pthread_cond_t  infoThreadCond ;
  int  requestFlag;
};

struct listenHash
{
    /*Value -1 means that the first entry is empty */
    int   fd;

    /*This is important because we want consistent stores.*/
    struct listenHash    *next __attribute__ ((aligned(32)));

    /*This is the list which indicates all the  connections that the listener has accepted*/
    struct listenHash    *liveNext, *livePrev __attribute__ ((aligned(32)));

    /*   
         1:Alive, 
         0:Closed
    */
    int isAlive;

    /*This field indicates  the temporary owner of a connection in case of pending conenctions*/
    pthread_t  tid;

    /*Specifies the index in the thread array which this node belongs to. -1 means its free for use*/
    int  threadArrIndex;

    /*0:Normal, 1:Blocked*/
    int  status;    
    
    /*0:Can Poll, 1: Cannot Pall*/
    int readPoll;
};


#include <inc/listener/listener_utils.h>


extern struct listenHash   socketHash[SocketArrayMax];
extern struct listenHash   *connectHead, *connectTail;
extern struct listenHash   liveConnections;
extern struct connectThreads_t   connectThreads[connectionThreadsMax];

void listenerStart();
int init_thread_connection();

#endif