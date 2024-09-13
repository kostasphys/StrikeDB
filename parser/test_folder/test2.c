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



struct whatever{
	int ak;
	char  bff[30];
};

int main() {

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
    

}