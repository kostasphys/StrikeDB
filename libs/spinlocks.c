#include  <inc/compiler.h>
#include  <inc/spinlocks.h>

static unsigned int rw_sub_and_test(int i, int *v);

#ifdef HAS_ATOMIC


/************************READ-WRITE SPINLOCKS*******************/

int try_read_spinlock_db(struct rw_spinlock *rw_lock) {

	atomic_dec(rw_lock);
	if (atomic_read(rw_lock) >= 0)
		return 1;

	atomic_inc(rw_lock);
	return 0;
}



void read_spinlock_db(struct rw_spinlock *rw_lock) {

    while( try_read_spinlock_db(rw_lock) == 0);
    
}



void release_read_rwlock_db(struct rw_spinlock *rw_lock)
{
    atomic_inc(rw_lock);
}


static unsigned int rw_sub_and_test(int i, int *v)
{
	unsigned int c;

	__asm__ __volatile__(
		"lock ;" "subl %2,%0; sete %1"
		:"=m" (*v), "=qm" (c)
		:"ir" (i), "m" (*v) : "memory");
	return c;
}


int try_write_spinlock_db(struct rw_spinlock *rw_lock) {
    
	if (rw_sub_and_test(RW_SET_BIT, &rw_lock->lock))
		return 1;

	atomic_add(RW_SET_BIT, &rw_lock->lock);
	return 0;
}


void write_spinlock_db(struct rw_spinlock *rw_lock) {

        while( try_read_spinlock_db(rw_lock) == 0);

}


void release_write_rwlock_db(struct rw_spinlock *rw_lock)
{
    atomic_add(RW_SET_BIT, &rw_lock->lock);
}


#else /*HAS_ATOMIC*/

/*TODO Later*/

#endif

int init_rw_spinlock(struct rw_spinlock *rw_lock )
{
    rw_lock->lock = RW_SET_BIT;
    return 0;

}


/************************END READ-WRITE SPINLOCKS*******************/

