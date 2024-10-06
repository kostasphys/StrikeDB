#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inc/strings.h>
#include <inc/nodes.h>
#include <sys/shm.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <inc/types/type_defs.h>
#include <inc/listener/listener.h>


struct whatever{
	int ak;
	char  bff[30];
};

int main() {
/*
    struct whatever buffer, obj;
    mqd_t mqd;

    struct mq_attr mattr, oldattr;

    //memset(&mattr,0, sizeof(struct mq_attr));


    mattr.mq_msgsize = 50;
    mattr.mq_maxmsg = 100;
    mattr.mq_flags = 0;
    mattr.mq_curmsgs = 0;
   // mattr.mq_flags = 0;




	mqd = mq_open("/novvel", O_RDWR);

    mq_getattr(mqd,&oldattr);
    perror("Error");
    
    mq_setattr(mqd, &mattr, &oldattr);

    printf("mqd %d", mqd);

    strcpy(obj.bff,"Receiver is sending");

    while(1)
    {   ssize_t j;
        int ff;
        j = mq_receive(mqd, &buffer, sizeof(struct whatever), NULL);
        printf("Errno: %d \n", errno);
     //   iret = mq_receive(queue, rcvmsg, 50, NULL);    
        //sleep(1);
      //  ff = mq_send(mqd, &obj, sizeof(struct whatever), NULL);
        printf("ff is %d \n", j);
        printf("buff : %s \n", buffer.bff);
        //sleep(1);
    }
    */
   struct connectThreadsInfo *header, *ptr, *dummy;

    header = malloc(sizeof(struct connectThreadsInfo));
    header->fd = 0;
    GEN_INIT_HEAD(header);
    

    for(int i=0; i < 9; ++i){
        struct connectThreadsInfo *ptr = malloc(sizeof(struct connectThreadsInfo));
        ptr->fd = i + 10;
        generic_add_list(header, ptr);
        
    }

    

    generic_del_item((header->next->next->next));
    generic_del_item((header->next->next->next));
    generic_del_item((header->next->next->next));
    generic_del_item((header->next->next->next));
    generic_del_item((header->prev));

    ptr = header->next;
    for(;;)
    {
        if(ptr == header)
        {
            break;
        }
        printf("Lulis %d\n", ptr->fd);
        ptr = ptr->next;
    }

}