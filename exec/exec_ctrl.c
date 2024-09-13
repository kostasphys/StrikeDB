#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <inc/exec/exec_threads.h>
#include <inc/IPC/mqueues.h>



void *ctrl_thread(void *arg)
{   
    mqd_t mqRx, mqTx;
    struct mqueue_msg msgBuffer;
    int ret;

    if ( open_mqueue("/exec_Rx", O_RDONLY) == -MQ_ERR )
    {
        printf("Error during opening Exec_Rx \n");
        perror("");
        exit(-1);
    }


    if ( open_mqueue("/exec_Tx", O_WRONLY) == -MQ_ERR )
    {
        printf("Error during opening Exec_Tx \n");
        perror("");
        exit(-1);
    }

    mqRx = sys_mqueues.mqueues[MQ_EXEC_RX_ID].id;
    mqTx = sys_mqueues.mqueues[MQ_EXEC_TX_ID].id;

    printf("CTRL INITIALIZED \n");

    while (1)
    {
      ret = read_mqueue(mqRx, &msgBuffer);
      if(ret == -MQ_ERR)
      {
        printf("Error while reading \n");
        perror("");
        exit(-1);
      }

      printf("Message CONTROL %s \n", msgBuffer.buff);

    }
    

}



void init_ctrl()
{

  if ( pthread_create(&sys_threads.threads[EXEC_CTRL_ID].tid, NULL, ctrl_thread, NULL) < 0 )
{
    printf("Error Initializing exec_wait_mess  Thread \n");
    exit(-1);
}

}