#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inc/list.h>
#include <inc/listener/listener.h>

struct  authWait_t    *authWait;
struct connectThreadsInfo *authConnTemp;

pthread_mutex_t listenAuthMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  listenAuthCond  = PTHREAD_COND_INITIALIZER;
int authRequest = 0;
char magic_number[] = "FFEEDD00";

void *authThreadFn(void *arg)
{
    /* for(;;)
    {
        pthread_mutex_lock(&listenAuthMutex);
        while(connectThreads[arrIndex].requestFlag == 0)
            pthread_cond_wait(&connectThreads[arrIndex].infoThreadCond, &connectThreads[arrIndex].infoThreadMutex);
        

    } */
}

void authThreadInit()
{
    
    authWait  = malloc(sizeof(struct authWait_t));
    authConnTemp = malloc(sizeof(struct connectThreadsInfo));
    if(authWait == NULL || authConnTemp)
    {
        fprintf(stderr, "Error when allocating authWait, authConnTemp \n");
        exit(-1);
    }

    INIT_HEAD(&(authWait->authList));
    memset(authConnTemp, '\0', sizeof(struct connectThreadsInfo));
    
}

int authorizeConn(int fd)
{
    int ret;
    char line[255];

    memset(authConnTemp, '\0', sizeof(struct connectThreadsInfo));
    authConnTemp->fd = fd;

    usleep(10000);
    ret = readMsgFast(authConnTemp->fd, &authConnTemp->pakcet, (int *)&authConnTemp->readBytes);
    if(ret == 0 )
    {
        fprintf(stderr, "The connection closed before authorizing it \n");
        return -1;
    }
    /*This needs fixing ASAP. When this happens we should give the connection to the authThread*/
    else if(ret < 0 )
    {
        sprintf(line, "The connection is blocked, for now we will decline those \n");
        trace_file(line);    
        return -1;
    }


    if( authConnTemp->pakcet.head.type != 0)
    {
        sprintf(line, "This is not an authorization message, wrong type:%d\n", authConnTemp->pakcet.head.type);
        trace_file(line);    
        return -1;
    }

    if(strcmp(authConnTemp->pakcet.buffer.buffer, magic_number))
    {
        sprintf(line, " Authorization declined. The magic number is wrong: %s \n", authConnTemp->pakcet.buffer.buffer);
        trace_file(line);    
        return -1;
    }


    sprintf(line, " Authorization Succeeded\n");
    trace_file(line);    

    return 0;

    
}