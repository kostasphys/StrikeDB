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
//char magic_number[] = AUTH_MAGIC_NUMBER;

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
    if(authWait == NULL || authConnTemp == NULL)
    {
        fprintf(stderr, "Error when allocating authWait, authConnTemp \n");
        exit(-1);
    }

    INIT_HEAD(&(authWait->authList));
    memset(authConnTemp, '\0', sizeof(struct connectThreadsInfo));
    
}

int authorizeErr(int err)
{

}


int authorizeConn(int fd, int retry)
{
    int ret;
    char line[255];
    int tempRet = retry;
    int magicLen = strlen(AUTH_MAGIC_NUMBER);
    char authString[magicLen + 1];
 
 
    memset(authString, '\0', sizeof(authString));
    memset(authConnTemp, '\0', sizeof(struct connectThreadsInfo));
    authConnTemp->fd = fd;


    usleep(10000);
    while(tempRet != 0)
    {
        
        ret = ReadMsgHead(authConnTemp->fd, &authConnTemp->packet, &authConnTemp->rwBytes);

        if(ret == 0 )
        {
            fprintf(stderr, "Header: The connection closed before authorizing it \n");
            return -1;
        }
        else if(ret > 0)
            break;
        

        --tempRet;
    }
    
    if(ret < 0 )
    {
        sprintf(line, "The connection is blocked, for now we will decline those \n");
        trace_file(line);    
        return -1;
    }


    if( authConnTemp->packet.head.type != 1)
    {
        sprintf(line, "This is not an authorization message, wrong type:%d , body:%s \n", authConnTemp->packet.head.type,authConnTemp->packet.buffer.buffer);
        trace_file(line);    
        return -1;
    }

    tempRet = retry;

    usleep(10000);
    while(tempRet != 0)
    {

        ret = readMsgFast(authConnTemp->fd, &authConnTemp->packet, (int *)&authConnTemp->rwBytes, 1);
        if(ret == 0 )
        {
            fprintf(stderr, "Body: The connection closed before authorizing it \n");
            return -1;
        }
        else if(ret  > 0)
            break;

        /*This needs fixing ASAP. When this happens we should give the connection to the authThread*/
        --tempRet;
    }

    if(ret < 0 )
    {
        sprintf(line, "The connection is blocked during body, for now we will decline those \n");
        trace_file(line);    
        return -1;
    }


    strncpy(authString, authConnTemp->packet.buffer.buffer, magicLen);

    if(strcmp(authString, AUTH_MAGIC_NUMBER))
    {
        sprintf(line, " Authorization declined. The magic number is wrong: %s \n", authConnTemp->packet.buffer.buffer);
        trace_file(line);    
        return -1;
    }


    sprintf(line, " Authorization Succeeded\n");
    trace_file(line);    

    return 0;

    
}