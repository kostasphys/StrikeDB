#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inc/exec/exec_threads.h>



void *exec_proc_mess(void *arg)
{

    while(1)
    {   
        TExecWaitMsg();

        TExecGetMsg(NULL);

        printf("Message received from thread %lu\n", pthread_self());

        TExecFreeEntry();

        processing_work();

    }

}



pthread_t trial_exec;

void *exec_wait_mess(void *arg)
{
    
    int i, j;
    for( i=0; i<EXEC_THREADS_NUM; ++i)
    {   
        j = EXEC_RESERVED_THREADS + i - 1;
        if ( pthread_create(&sys_threads.threads[j].tid, NULL, exec_proc_mess, NULL) < 0 )
        {
            printf("Error Initializing exec_wait_mess  Thread \n");
            exit(-1);
        }

        printf("Work threads initialized with %lu\n", sys_threads.threads[ EXEC_RESERVED_THREADS + i - 1 ].tid );
        
    }
    
    printf("After loop \n");

    while(1)
    {
        //We will change this later
        sleep(10);
        printf("New message \n");

        TExecWaitEntry();

        TExecPutMsg(NULL);

        TExecInfoWork(NULL);

    }
    
}