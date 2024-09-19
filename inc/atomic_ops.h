#ifndef    __s_atomic_ops_h__
#define    __s_atomic_ops_h__

/*************************************/
void atomic_add_db(int i, int *v);
void atomic_inc_db(int *v);
void atomic_dec_db(int *v);
int atomic_read_db(int *v);
/*************************************/


#endif