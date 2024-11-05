#ifndef   __s_listener_threads_h__
#define   __s_listener_threads_h__

#include <inc/threads.h>
#include <inc/list.h>

#define pollingThreads          1
#define pollingThreadsMax       256
#define connectionThreadsMax    1024
#define connectionThreads       4
#define tempMemPools            128

#define SocketArrayMax          1024

/***************************************/
void *pollingThreadFn(void *arg);
void *authThreadFn(void* arg);
void authThreadInit();
int  authorizeConn(int fd);
/***************************************/

extern pthread_mutex_t livePollMutex;
extern pthread_cond_t  livePollCond;
extern int  liveConnects;

struct authWait_t {
    int fd;
    struct list_head    authList;

};

extern struct  authWait_t    *authWait;

#endif