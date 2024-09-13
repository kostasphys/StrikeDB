#ifndef     __s_nodes_h__
#define     __s_nodes_h__

#include  <inc/nodes_defs.h>
#include  <inc/bitmap.h>


int find_qnode(struct token *tk);
unsigned int find_hash_node(char *name);


struct qnode {
	
	char name[255];

	/*This indicates the position 
	  inside the qnode table
	*/
	unsigned int shift_op;

	/*0 for DML, 1 for DDL*/
	struct bitmap_node bitmap_arr[2]; 

	/* Indicates if this is a starter keyword of a query
	   0: Usual keyword
	   1: DML Start (Select, Insert..)	
	   2: DDL Start (Create,Drop)
	*/
	int  qstart;  
};



struct hash_node {
	struct qnode *qptr;
	struct hash_node  *next;
};


extern struct qnode nodes_array[P_MAXELM];
extern struct hash_node hash_node_list[HASH_MAX];

extern int hash_list_init();

#endif
