#include <stdio.h>
#include <pthread.h>
#include <inc/exec/exec_threads.h>


static pthread_mutex_t syncQueueMutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t syncReadMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  syncReadCond   = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t syncWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  syncWriteCond  = PTHREAD_COND_INITIALIZER;

static int itemGet;
static int itemPut;
static int queueSlots = 8;
static int queueItem ;


int TExecWaitEntry()
{
   pthread_mutex_lock(&syncWriteMutex);
   while (queueSlots <= 0)
       pthread_cond_wait(&syncWriteCond, &syncWriteMutex);
   queueSlots--;
   pthread_mutex_unlock(&syncWriteMutex);
   return 1;
}


int TExecInfoWork()
{
   pthread_mutex_lock(&syncReadMutex);
   queueItem++;
   pthread_cond_signal(&syncReadCond);
   pthread_mutex_unlock(&syncReadMutex);
    return 1;
}

int TExecWaitMsg()
{
   pthread_mutex_lock(&syncReadMutex);
   while (queueItem <= 0)
        pthread_cond_wait(&syncReadCond, &syncReadMutex);
   queueItem--;
   pthread_mutex_unlock(&syncReadMutex);
    return 1;
}

int TExecPutMsg(void *ptr)
{
   pthread_mutex_lock(&syncQueueMutex);
  // memcpy ((char *) &(tab_BalDemande[itemPut]), (char *) sBalMsg,                                            sizeof(TSHsmExchange));

   itemPut = (itemPut +1) % ( max_request);
   pthread_mutex_unlock(&syncQueueMutex);
   return 1;
}


int TExecGetMsg(void *ptr)
{
   pthread_mutex_lock(&syncQueueMutex);
//   memcpy ((char *) sBalMsg, (char *) &(tab_BalDemande[itemGet]),                                              sizeof(TSHsmExchange));
//   memset ((char *) &(tab_BalDemande[itemGet]),0,sizeof(TSHsmExchange));*/
  

   itemGet = (itemGet +1) % (max_request);
   pthread_mutex_unlock(&syncQueueMutex);
    return 1;
}

int TExecFreeEntry()
{
   pthread_mutex_lock(&syncWriteMutex);
   queueSlots++;
   pthread_cond_signal(&syncWriteCond);
   pthread_mutex_unlock(&syncWriteMutex);
    return 1;
}

