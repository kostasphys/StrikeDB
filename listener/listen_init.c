#include <stdio.h>
#include <stdlib.h>
#include <inc/IPC/mqueues.h>
#include <inc/listener/listener.h>
#include <inc/net/tcp.h>

void create_mqueues()
{
    pack_mqueue("/listen_Rx", MQ_LISTEN_RX_ID);
    pack_mqueue("/listen_Tx", MQ_LISTEN_TX_ID);
}

void open_mqueues()
{
    open_mqueue("/exec_Rx", O_WRONLY);
    open_mqueue("/exec_Rx", O_RDONLY);
}

void startComms()
{
    int ret;
    ret = InitListen(listenerPort);
    
    if(ret <= -1)
    {
        printf("Abort listener... \n");
        exit(-1);
    }

}


void listener_init()
{
    create_mqueues();
    open_mqueues();

    startComms();

    //init_thread_connection();
    
}