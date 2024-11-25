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
#include <errno.h>

int listenerFd;


struct test_test
{
  int id;
  char name[255];
};

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


/* int  test_func()
{
  printf("Test Works \n");
  return 0;
} */



void fn_handle(int x)
{
  printf("HANDLER RAN!! \n");
}

mqd_t  Rx;



int main(int argc, char **argv){
    char var;
    int fd, ret;
    struct connectThreadsInfo packet;
    struct sockaddr_in sAddrClient;
    struct mqueue_msg msgBuffer;
    struct line_msg buffer;

    char opts = 'x';

    if (argc <= 1  )
    {
      printf("Too few vals \n");
      exit(-1);
    }

    opts = argv[1][0];


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
    //sAddrClient.sin_addr.s_addr = inet_addr("192.168.1.58");



    ret = connect(fd, (struct sockaddr*)&sAddrClient, sizeof(struct sockaddr));

    if(ret == -1)
    {
      printf("Cant connect \n");
      return -1;
    }

    printf("Client connected \n");

    memset(&packet, '\0', sizeof(packet));

    char tempBuff[255];
    int cc = 0;

    if(opts == 'a')
    {
      packet.packet.head.type = 1;
      strcpy(packet.packet.buffer.buffer, AUTH_MAGIC_NUMBER);
      packet.packet.head.size = strlen(packet.packet.buffer.buffer);
    }
    else if(opts == 'b')
    {
      packet.packet.head.type = 1;
      strcpy(packet.packet.buffer.buffer, "ASKkkmcmhkjljhlkjhljhsnfkdalsfhieqfuqiqkdvlcdnlqoehpuqfhpuvnksdjfvjkalsdhvabjkasldfjh");
      packet.packet.head.size = strlen(packet.packet.buffer.buffer);  
    }
    else
      packet.packet.head.type = 544;

 /* 
 */
    ret = writeMsgFast(fd, &packet.packet, &packet.rwBytes);
    if(ret <= 0 )
    {
      printf("ERROR when Writing for auth \n");
      return 0;
    }


  for(;;)
  {

    strcpy(buffer.buffer, strz);

    sprintf(tempBuff, "Tilter here is calling you %d, pid:%d", cc, getpid() );
   // trace_file(tempBuff)
    packet.packet.head.size = strlen(tempBuff);
   // packet.packet.head.size = 542393671;
    memcpy(packet.packet.buffer.buffer, tempBuff, packet.packet.head.size);



  
retry:
    ret = writeMsgFast(fd, &packet.packet, &packet.rwBytes);
    if(ret == 0)
    {
      sprintf(strz, "Connection failed, we close the client \n" );
      strcpy(buffer.buffer, strz);
      close(fd);
      return -1;
    }
    else if( ret == -EAGAIN || ret == -EINTR)
    {
      sprintf(strz, "Write is blocked... We try again \n" );
      trace_file(strz);
      perror("");
      goto retry;
    }
    else if(ret < 0)
    {
      sprintf(strz, "General error \n" );
      trace_file(strz);
      perror("");
      close(fd);
      return -1;
    }

    printf("Message sent: %d \n", cc);

    
    printf("Sleep a little bit \n");
    usleep(5*pow(10,4));
  
    
    ++cc;
    if((cc)%(15) == 0)
    {
      usleep(6*pow(10,5));
      
    }

    memset(&packet, '\0', sizeof(packet));
    
  }
  

  pause();
}