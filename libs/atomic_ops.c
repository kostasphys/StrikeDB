
void smp_mb()
{
	int v,i;
	__asm__ __volatile__(
		"lock ; " "addl %1,%0"
		:"=m" (v)
		:"ir" (i), "m" (v): "memory");
}

void atomic_add_db(int i, int *v)
{
	__asm__ __volatile__(
		"lock ; " "addl %1,%0"
		:"=m" (*v)
		:"ir" (i), "m" (*v): "memory");
}


void atomic_set_db(int *v, int i)
{
	
}


void atomic_inc_db(int *v)
{
	__asm__ __volatile__(
		"lock ; " "incl %0"
		:"=m" (*v)
		:"m" (*v): "memory");
}



void atomic_dec_db(int *v)
{
	__asm__ __volatile__(
		"lock ; " "decl %0"
		:"=m" (*v)
		:"m" (*v): "memory" );
}





int atomic_read_db(int *v)
{
    int var = 0;

    __asm__ __volatile__(
		"lock ; " "addl %1, %0"
		:"=m" (var)
		:"r" (*v): "memory");


    return var;
}