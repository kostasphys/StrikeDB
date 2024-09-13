#include <stdio.h>
#include <inc/bitops.h>



/*
Tests the bit in the pos position inside the 
bitarr array.

bitarr: bitmask
pos: the absolute position inside the mask. 

*/

int test_bit_abs(unsigned long *bitarr, unsigned int pos)
{
    int i;

    /*
    Find the index which this bit belong inside 
    the arr
    */
    i = ( BITS_TO_uLONG(pos) )- 1 ;
    
    pos = pos%(BITS_PER_uLONG);

    printf("POS before  %u, %lu \n\n", pos, BITS_PER_uLONG);  
    printf("BITMAP IS: %lu \n\n", bitarr[0]);

    if(bitarr[i] & (1 << pos))
        printf("PASSED\n");

    return  ( bitarr[i] & (1 << pos) );
}


