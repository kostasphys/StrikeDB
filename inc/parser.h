#ifndef    __s_parser_h__
#define    __s_parser_h__

   #include <inc/list.h>
  
   #define DDL_COUNT 3
   #define DML_COUNT 9	


   struct token {
       
       char name[255];

	   unsigned  short  name_size;

	   struct  list_head  token_list;

       struct  qnode  *qnode;
       
       int  id;           
   };	
		
#endif
