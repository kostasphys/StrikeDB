#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <inc/listener/listener_threads.h>

struct dbConnections{

};

int setFdMask()
{
    for(;;)
    {

    }
}

void *pollingThreadFn(void *arg)
{
    fd_set readfds;
    int sfd, ret;

    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(1, &readfds);

        sfd = 0;

        ret = select(sfd, &readfds, NULL, NULL, NULL);

        if( ret == -1)
        {
            printf("Error while selecting \n");
            exit(-1);
        }


    }
}