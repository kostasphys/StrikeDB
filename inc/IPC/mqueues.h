#ifndef     __s_mqueues_h__
#define     __s_mqueues_h__

#include  <inc/messages.h>
#include  <mqueue.h>

#define  MAX_QUEUE_NUM    10
#define  BUFFER_SIZE  sizeof(struct mqueue_msg)

#define OPEN_FLAGS    O_CREAT|O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

/******ERROR MESSAGES*********/
#define  MQ_OK          0
#define  MQ_ERR         1   // General Error
#define  MQ_SIZE        2
#define  MQ_NOTME       3
#define  MQ_ERRREAD     4
#define  MQ_ERRWRITE    5
#define  MQ_INVALID     6

/*******Communication IDs******/
#define  EXEC_ID_RX         1
#define  EXEC_ID_TX         2
#define  EXEC_ID_RX_TX      3
#define  EXEC_ID_MAIN       4
#define  EXEC_ID_EXTERN     5

#define  PARSER_ID_RX       5
#define  PARSER_ID_TX       6

/********** Mqueue Index IDs******/
#define  MQ_EXEC_RX_ID            0
#define  MQ_EXEC_TX_ID            1
#define  MQ_LISTEN_RX_ID          2
#define  MQ_LISTEN_TX_ID          3

/*********MESSAGE TYPES***********/
#define  MQ_TEST         1
#define  MQ_PING         2   
#define  SQL_CMD         3
#define  SQL_CMD_ACK     4



struct skMqueue_struct_info {

    char name[255];
    mqd_t   id;
    
};

struct skMqueue_Struct {
    struct skMqueue_struct_info mqueues[MAX_QUEUE_NUM];
};

extern  struct  skMqueue_Struct  sys_mqueues;

int create_mqueue(char *name);
int empty_mqqueue(mqd_t mq);
int write_mqueue(mqd_t mq, struct mqueue_msg *buff);
int read_mqueue(mqd_t mq, struct mqueue_msg *buff);
void pack_mqueue(char *str, int idx);
mqd_t open_mqueue(char *str, long int flags );


#endif