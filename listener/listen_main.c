#include <stdio.h>
#include <inc/messages.h>
#include <inc/listener/listener.h>
#include <inc/net/tcp.h>
#include <inc/IPC/mqueues.h>
#include <inc/listener/listener_threads.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int  listenerFd, newFd;
struct line_msg msg_buffer;
struct  skMqueue_Struct  sys_mqueues;
pthread_t   tid;


struct connectThreads_t   connectThreads[connectionThreadsMax];
struct listenHash   socketHash[SocketArrayMax];


int main()
{
    listener_init();
/*
    int i;
    for(i=0; i < pollingThreads; i++)
    {
        if ( pthread_create(&tid, NULL, pollingThreadFn, NULL) < 0 )
        {
            printf("Error Initializing Polling  Threads \n");
            exit(-1);
        }
    }
*/    
    

    
   // while(1)
  //  {
       newFd = acceptConnect();
       if(newFd == -1)
       {
            printf("Error while accepting a new connection \n");
       }
   // }
    printf("Pockie Listener accepted the call \n\n");

    int x;
    x=fcntl(newFd ,F_GETFL, 0);
    fcntl(newFd, F_SETFL, x | O_NONBLOCK);

    struct line_packet packet;
    int readBytes = 0;
     
    while(1)
    {
        printf("**************************** \n");
        int pp =  readMsgFast(newFd, &packet, &readBytes);
        printf("Error message :%d, ReadBytes: %d \n\n\n\\n", pp, readBytes);
        //readMsg(newFd, &msg_buffer);

        if (pp != 11)
            readBytes = 0;
    
        printf("client Message: %s \n", packet.buffer.buffer);
        printf("****************************\n\n\n\n\n");
        memset(&packet,'\0', sizeof(packet));

        sleep(4);

    }
     
}