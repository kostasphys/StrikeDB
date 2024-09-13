#ifndef   __s_bitops_h__
#define   __s_bitops_h__
 

#define BITS_PER_uLONG   8*sizeof(unsigned long)

#define BITS_TO_uLONG(x) \
	x <= BITS_PER_uLONG ? 1 : (x/BITS_PER_uLONG) + 1 




int test_bit_abs(unsigned long *bitarr, unsigned int pos);

#endif
