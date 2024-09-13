#include <stdio.h>
#include <inc/IPC/mqueues.h>
#include <string.h>

mqd_t  Rx;

int main(){
    char var;
    struct mqueue_msg msgBuffer;

    Rx =  open_mqueue("/exec_Rx", O_WRONLY);

    if(Rx < 0 )
    {
        perror("");
        printf("Error Opening \n ");
        return -1 ;
    }

    strcpy(msgBuffer.buff, "fakofit");

    while(1)
    {   
        printf("Enter command \n");

        scanf("%c", &var);
        getchar();

        printf("Good \n");
        
        msgBuffer.buff[0] = var;

        write_mqueue(Rx, &msgBuffer);

    }
    
}