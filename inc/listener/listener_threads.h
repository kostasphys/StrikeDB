#ifndef   __s_listener_threads_h__
#define   __s_listener_threads_h__

#include <inc/threads.h>

#define pollingThreads          1
#define pollingThreadsMax       256
#define connectionThreadsMax    1024
#define connectionThreads       4
#define tempMemPools            128

#define SocketArrayMax          1024

void *pollingThreadFn(void *arg);
extern pthread_mutex_t livePollMutex;
extern pthread_cond_t  livePollCond;
extern int  liveConnects;

#endif