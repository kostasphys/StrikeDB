#include <stdio.h>
#include <stdlib.h>

#define __USE_POSIX


#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>
#include <inc/IPC/mqueues.h>
#include <inc/listener/listener.h>
#include <inc/net/tcp.h>
#include <inc/debug.h>


char msg[255];


int      install_handler(int signal, void (*processing)(int))
{
   int       Return;
   struct    sigaction mask;

   mask.sa_handler = processing;
   sigemptyset(&mask.sa_mask);
   mask.sa_flags = SA_SIGINFO;

   Return = sigaction(signal, &mask, NULL);
   if ( Return < 0 )
   {
      fprintf(stderr, "Probelom installing handler \n");
      return -1;
   }

   return 1;
}

void fn_handle(int x)
{
  char line[255];

  sprintf(line, "SIGNAL HANDLER FOR USR2 has been executed \n");
  trace_file(line);

}

void fn_handle_int(int x)
{
  printf("GOODBYE!! \n");
  exit(0);
}



void  ThdMaskSignals()
{

	sigset_t   set;

	sigemptyset(&set);
//	sigaddset(&set, SIGPIPE);
//	sigaddset(&set, SIGCHLD);
	sigaddset(&set, SIGUSR2);

	pthread_sigmask(SIG_BLOCK, &set, NULL) ;

}

void create_mqueues()
{
    pack_mqueue("/listen_Rx", MQ_LISTEN_RX_ID);
    pack_mqueue("/listen_Tx", MQ_LISTEN_TX_ID);
}

void open_mqueues()
{
    open_mqueue("/exec_Rx", O_WRONLY);
    open_mqueue("/exec_Rx", O_RDONLY);
}

void startComms()
{
    int ret;
    ret = InitListen(listenerPort);
    
    if(ret <= -1)
    {
        fprintf(stderr, "Abort listener... \n");
        exit(-1);
    }

}


void listenerStart()
{
    int Return;

    create_mqueues();

    open_mqueues();

    
    install_handler(SIGUSR2, fn_handle);
    install_handler(SIGINT, fn_handle_int);
    
    /*Open trace file*/
    Return = open_trace(O_TRUNC);
    if(Return < 0)
    {
        fprintf(stderr, "Can't open/create listen_trace file. Abort \n");
        exit(-1);
    }

    /*Initialize authThread structs*/
    authThreadInit();

    /*Start the listener*/
    startComms();

    init_hash_sock();
    
    /*Initialize the connection threads which handles all the network load*/
    init_thread_connection();
    
}