#ifndef   __s_listener_h__
#define   __s_listener_h__

#include <inc/list.h>
#include <inc/threads.h>
#include <inc/listener/listener_threads.h>
#include <sys/types.h>
#include <inc/messages.h>

#define listenerPort 7000

struct connectThreadsInfo
{   
    int fd;
    pthread_t tid;

    struct connectThreadsInfo   *prev, *next;
    /* If we cant read the whole message in one  trial then we store the already bytes here */
    int       isBlocked;
    struct line_packet    pakcet;
    ssize_t     readBytes;
    size_t      msgSize;
    /*This is for the listener thread. We need an efficient way to communicate the status of the socket*/
    struct listenHash   *hashNode;    

};

struct connectThreads_t
{
  //struct connectThreadsInfo SocketArr[SocketArrayMax];
  struct connectThreadsInfo *pendingRequests;
  struct connectThreadsInfo *tempRequests;
  pthread_mutex_t infoThreadMutex ; 
  pthread_cond_t  infoThreadCond ;
  int  requestFlag;
};

struct listenHash
{
    /*Value -1 means that the first entry is empty */
    int   fd;
    struct listenHash    *next;
    /*This field indicates  the temporary owner of a connection in case of pending conenctions*/
    pthread_t  tid;
    /*Specifies the index in the thread array which this node belongs to. -1 means its free for use*/
    unsigned int  threadArrIndex;
    /*0:Normal, 1:Blocked*/
    int  status;
};

extern struct listenHash   socketHash[SocketArrayMax];
extern struct connectThreads_t   connectThreads[connectionThreadsMax];

void listener_init();
int init_thread_connection();

#endif