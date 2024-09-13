#ifndef      __s_spinlocks_h___
#define      __s_spinlocks_h___

#include   <inc/atomic_ops.h>
#include   <inc/types/types_size.h>

#define    RW_SPIN_SHIFT    __S_SIZE_INT__ * 4

#define    RW_SET_BIT     ( 1 << RW_SPIN_SHIFT )  



struct rw_spinlock {
    int lock;
};


struct gen_spinlock {
    int lock;
};

/********************************************************/
extern  int  init_rw_spinlock(struct rw_spinlock *rw_lock );
extern  int  write_spinlock();
extern  int  read_spinlock();
extern  void atomic_dec(int *v);
/********************************************************/



#endif