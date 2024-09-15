#include <stdio.h>
#include <inc/messages.h>
#include <inc/listener/listener.h>
#include <inc/net/tcp.h>
#include <inc/IPC/mqueues.h>

int  listenerFd, newFd;
struct line_msg msg_buffer;
struct  skMqueue_Struct  sys_mqueues;

int main()
{
    listener_init();

   // while(1)
  //  {
       newFd = acceptConnect();
       if(newFd == -1)
       {
            printf("Error while accepting a new connection \n");
       }
   // }
    printf("Listener accepted the call \n\n");

    while(1)
    {
        readMsg(newFd, &msg_buffer);
        printf("Message from client is %s \n", msg_buffer.buffer);

    }
     
}