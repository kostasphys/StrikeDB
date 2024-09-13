#ifndef     __s_tables_h__
#define     __s_tables_h__

#include <pthread.h>
#include <inc/list.h>
#include <inc/spinlocks.h>
#include <inc/schedule.h>

/* Size in MBs */
#define BLOCK_SIZE    4096

#define BLOCK_HEADER_SIZE

/*Number of Blocks inside extend*/
#define EXTEND_NUM    32

#define EXTEND_SIZE   BLOCK_SIZE*EXTEND_NUM


#define NAME_SIZE     255

#define ROW_HEADER_SIZE     

/*
        BLOCK HEAD ANATOMY:

        A block is composed of a head and its body.

        The head  starts with a bitmap that indicates the free rows
        inside the block. The size varies based on ratio between the size of
        the block and the size of the column.

        The next byte encodes the following information:
            bit 0 : This is the lock of the row.  
            bits 1-7 :  States which preceding transaction has scanned the row.
                        These bits makes sure that no deadlocks can happen because of the
                        pending writers.


*/

struct mem_block {


};

struct extend_desc {

    /* 
       Lock that synchronizes readers and writers.
       Only one writer is permitted at a time but 
       many readers can acquire this.
    */
    struct rw_spinlock  ext_lock;

    struct list_head   next;

    /*The system counter when this extend was created*/
    sys_counter_t   create_tik;

    /*The last transaction that modified this extend*/
    sys_counter_t   trans_tik;


};


struct table_desc {

    /* This lock  synchronizes DDLs with DMLs */
    pthread_spinlock_t big_lock;

    /*Only when adding or reclaiming extends*/
    pthread_spinlock_t ins_del_lock;

    unsigned long flags;

    char name[NAME_SIZE];

    unsigned long row_size;

    /*
      This points to the first element in the 
      list_extend list
    */
    struct list_head  list_extend;

    /*Size of table in extends*/
    unsigned int num_extend;

};



#endif