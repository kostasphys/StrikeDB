#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inc/IPC/mqueues.h>
#include <inc/exec/exec_threads.h>



void create_mqueues()
{
    pack_mqueue("/exec_Rx", MQ_EXEC_RX_ID);
    pack_mqueue("/exec_Tx", MQ_EXEC_TX_ID);
}

void init_exec()
{
    create_mqueues();

    init_ctrl();

}