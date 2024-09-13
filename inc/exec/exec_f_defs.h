#ifndef     __s_exec_f_defs_h__
#define     __s_exec_f_defs_h__

#include   <mqueue.h>
#include   <inc/messages.h>

void *exec_wait_mess(void *);
void *exec_proc_mess(void *);
int TExecWaitEntry();
int TExecInfoWork();
int TExecWaitMsg();
int TExecPutMsg(void *ptr);
int TExecGetMsg(void *ptr);
int TExecFreeEntry();
void process_commnad();
void processing_work();
void init_exec();
void init_ctrl();
void *signal_thread(void *);
int read_exec_main(mqd_t mqTx, struct mqueue_msg *, int );
int read_exec_Rx(mqd_t Rx, mqd_t Tx);
int write_exec_Rx(mqd_t mqTx, struct mqueue_msg *msg_buff, int type, int rcv_id );
int write_exec_Tx(mqd_t, struct mqueue_msg *, int );
void init_ctrl_threads();
int write_mqueue(mqd_t mq, struct mqueue_msg *buff);

#endif




