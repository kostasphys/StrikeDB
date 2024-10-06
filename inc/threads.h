#ifndef __s_threads_h__
#define __s_threads_h__

#include <pthread.h>

#define MAX_SYSTEM_THREADS  32


#define   PARSER_THREAD_ID  0
#define   EXEC_THREAD_ID    1


struct skThread_Info {

    pthread_t   tid;
    int    idx;


};

typedef struct {

    struct skThread_Info  threads[MAX_SYSTEM_THREADS];
} skThread_Struct_t;



extern  skThread_Struct_t sys_threads;


void  ThdMaskSignals();

#endif