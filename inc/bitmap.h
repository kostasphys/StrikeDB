#ifndef   __s_bitmap_h__
#define   __s_bitmap_h__

#include  <inc/bitops.h>
#include  <inc/nodes.h>


#define DECLARE_BITMAP(name, size) \
	unsigned long name[BITS_TO_uLONG(size)]


struct bitmap_node {
	 DECLARE_BITMAP(bitmap, P_MAXELM); 
};

/*
struct bitmap_node {
	 unsigned long bitmap[10]}; 
 */
#endif

