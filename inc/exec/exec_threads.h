#ifndef     __s_exec_threads_h__
#define     __s_exec_threads_h__ 


#include <inc/threads.h>
#include <inc/exec/exec_f_defs.h>
#include <inc/exec/exec_var_defs.h>

#define EXEC_THREADS_NUM    4

#define EXEC_RESERVED_THREADS    4   /* From 0 - RESERVED_THREADS its reserved for each bin*/

#define  EXEC_WAIT_MESS_ID   0
#define  EXEC_CTRL_ID        1
#define  EXEC_SIG_ID         2 

#define max_request  EXEC_THREADS_NUM*2

/*This is the threshold for the erroneous messages 
in INT queue*/
#define READ_ERR_COUNT   1000

typedef  struct threads_descr  exec_threads;

struct ExecArgs{
    int indx;
};

struct exec_message {
    char* dummy;
};


#endif