#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inc/exec/exec_threads.h>
#include <inc/IPC/mqueues.h>


skThread_Struct_t  sys_threads ;
struct  skMqueue_Struct  sys_mqueues;


int main() {


init_exec();

  if ( pthread_create(&sys_threads.threads[EXEC_WAIT_MESS_ID].tid, NULL, exec_wait_mess, NULL) < 0 )
{
    printf("Error Initializing exec_wait_mess  Thread \n");
    exit(-1);
}


printf("Thread Exec_wait_mess init with tid %lu \n", sys_threads.threads[EXEC_WAIT_MESS_ID].tid);


while(1)
    pause();

}