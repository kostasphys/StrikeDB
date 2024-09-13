#include  <stdio.h>
#include  <sys/stat.h>
#include  <errno.h>
#include  <inc/IPC/mqueues.h>


unsigned long self_id ;

int create_mqueue(char *name)
{   
    struct mq_attr mattr;
    mqd_t mq;

   mattr.mq_msgsize = sizeof(struct mqueue_msg);
   mattr.mq_maxmsg =  10;
   mattr.mq_flags = 0;
   mattr.mq_curmsgs = 0;

   mq = mq_open(name, OPEN_FLAGS, &mattr);
   if ( mq < 0 )
        return -MQ_ERR;
   
   return mq;

}

mqd_t open_mqueue(char *str, long int flags )
{
    mqd_t mq;
    mq = mq_open(str, flags);

     if ( mq < 0 )
    {
        perror("");
        return -MQ_ERR;
    }

    return mq;

}


int read_mqueue(mqd_t mq, struct mqueue_msg *buff)
{
    ssize_t ret;

    ret =  mq_receive(mq, (void *)buff, BUFFER_SIZE, NULL);

     if( ret < 0)
    {
        perror("Err: ");
        return -MQ_ERR;
    }

/*
    if( buff->id != self_id )
        return -MQ_NOTME;
*/    

    return MQ_OK;

}



int empty_mqqueue(mqd_t mq)
{
    struct mqueue_msg *msg;
    struct mq_attr old_mattr, new_mattr;

    printf("MQ BEFOOOOORE %d ", mq);
    if( mq_getattr(mq, &old_mattr) == -1 )
    {
        perror("Error");
        printf("Error during retreiving MQ flags \n");
        return -MQ_ERR;
    }

    new_mattr.mq_flags = old_mattr.mq_flags;
    new_mattr.mq_flags |= O_NONBLOCK;
    
    if( mq_setattr(mq,&new_mattr ,&old_mattr) == -1 )
    {
        printf("Error during setting MQ flags \n");
        return -MQ_ERR;
    }

    /*Empty the queue*/
    while (mq_receive(mq, (void *)msg, BUFFER_SIZE, NULL) >= 0);

    if(errno != 11)
        return -MQ_ERRREAD;

    if( mq_setattr(mq,&old_mattr ,&new_mattr) == -1 )
    {
        printf("Error during setting MQ flags \n");
        return -MQ_ERR;
    }

    return MQ_OK;

}


void pack_mqueue(char *str, int idx)
{
    mqd_t fd;
    fd = create_mqueue(str);

    if ( fd == -MQ_ERR ) {
        perror("");
        printf("Error during creating queue %s ", str );
        exit(-1);
    }

    if (empty_mqqueue(fd) != MQ_OK) {
        perror("");
        printf("Error during emptying queue %s ", str );
        exit(-1);
    }


    sys_mqueues.mqueues[idx].id = fd;
    strcpy(sys_mqueues.mqueues[idx].name, str);
    
}


int write_mqueue(mqd_t mq, struct mqueue_msg *buff)
{
    int ret;
    ret = mq_send(mq, (void *)buff, BUFFER_SIZE, NULL);

    if( ret < 0 )
    {
        perror("Err: ");
        return -MQ_ERRWRITE;
    }

    return MQ_OK;

}