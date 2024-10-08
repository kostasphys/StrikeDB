#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define  __USE_POSIX
#include <signal.h>
#include <arpa/inet.h>
#include <inc/IPC/mqueues.h>
#include <inc/listener/listener.h>
#include <inc/net/tcp.h>
#include <inc/debug.h>
#include <inc/atomic_ops.h>
#include <math.h>

int listenerFd;


int      install_handler(int signal, void (*processing)(int))
{
   int       test_install_handler;
   struct    sigaction mask;



   mask.sa_handler = processing;
   sigemptyset(&mask.sa_mask);
   mask.sa_flags = SA_SIGINFO;

   test_install_handler = sigaction(signal, &mask, NULL);
   if (test_install_handler < 0)
   {
      printf("Probelom installing handler \n");
      return -1;
   }

   return 1;
}

void fn_handle(int x)
{
  printf("HANDLER RAN!! \n");
}

mqd_t  Rx;

int main(){
    char var;
    int fd, ret;
    struct sockaddr_in sAddrClient;
    struct mqueue_msg msgBuffer;
    struct line_msg buffer;

    int kilo = 1;
    
    char strz[500];


    
    install_handler(SIGSYS, fn_handle);
    install_handler(SIGTERM, fn_handle);
    install_handler(SIGABRT, fn_handle);
    
  //  install_handler(SIGKILL, fn_handle);
    

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if(fd == -1 )
    {
        printf("Error while creating socket \n");
        return -1;
    }

    ret = open_trace(0);
    if( ret == -1)
    {
      printf("Error while opening trace file \n");
      exit(-1);
    }

  


    bzero((char *)&sAddrClient, sizeof(sAddrClient));
    sAddrClient.sin_family = AF_INET;
    sAddrClient.sin_port   = htons(7000);
    sAddrClient.sin_addr.s_addr = inet_addr("161.35.165.25");
   // sAddrClient.sin_addr.s_addr = inet_addr("192.168.1.58");
/*
    Rx =  open_mqueue("/exec_Rx", O_WRONLY);

    if(Rx < 0 )
    {
        perror("");
        printf("Error Opening \n ");
        return -1 ;
    }

    strcpy(msgBuffer.buff, "fakofit");

    while(1)
    {   
        printf("Enter command \n");

        scanf("%c", &var);
        getchar();

        printf("Good \n");
        
        msgBuffer.buff[0] = var;

        write_mqueue(Rx, &msgBuffer);

    }
   */
  ret = connect(fd, (struct sockaddr*)&sAddrClient, sizeof(struct sockaddr));

  if(ret == -1)
  {
    printf("Cant connect \n");
    return -1;
  }

  printf("Client connected \n");


 // sleep(500);
  int cc = 0;
  while(1)
  {
    sprintf(strz, "Client here is calling you %d, pid:%d", cc, getpid() );
    strcpy(buffer.buffer, strz);
    
    
    writeMsg(fd, &buffer);
    printf("Sleep a little bit \n");
    
    usleep(6*pow(10,5));
  //sleep(1);
    
    /*
    if(cc == 1000)
    {
      printf("we sleep now ten SECS\n");
      sleep(10);
    }
      */

    ++cc;
    if((cc)%(15) == 0)
    {
      sleep(10);  
      
    }

    
    
  }
  

  pause();
}