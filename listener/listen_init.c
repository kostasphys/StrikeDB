#include <stdio.h>
#include <inc/IPC/mqueues.h>

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

void listen_init()
{
    create_mqueues();
    open_mqueues();

    
}