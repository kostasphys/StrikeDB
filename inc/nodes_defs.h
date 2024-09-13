#ifndef     __s_node_defs__
#define     __s_node_defs__


#define  P_CREATE     0
#define  P_DROP       1
#define  P_TABLE      2     //This is the "TABLE" keyword.
#define  P_SELECT     3
#define  P_DELETE     4
#define  P_FROM       5
#define  P_WHERE      6
#define  P_FIELD      7
#define  P_STAR       8
#define  P_COMMA      9
#define  P_LPARENTH   10
#define  P_RPARENTH   11
#define  P_LBRACKS    12
#define  P_RBRACKS    13
#define  P_AND        14
#define  P_OR 	      15
#define  P_EQUAL      16
#define  P_LEQ	      17
#define  P_GEQ 	      18
#define  P_LESS       19
#define  P_GREATER    20
#define  P_ADD 	      21
#define  P_DEL	      22 
#define  P_ORDER      23
#define  P_BY	      24
#define  P_TABNAME    25   //String of Table name.
#define  P_UPDATE     26  
#define  P_INSERT     27
#define  p_CONSTANT   28
#define  P_TYPE       29
#define  P_VARIABLE	  30
#define  P_ENDCHAR    31
#define  P_MAXELM     32


// This one is used to callibrate the linked listed in 
// the hash table.
#define  HASH_MAX     2*P_MAXELM  


#define  S_CREATE    (1 << P_CREATE )
#define  S_DROP      (1 << P_DROP )
#define  S_TABLE     (1 << P_TABLE )	
#define  S_SELECT    (1 << P_SELECT )
#define  S_DELETE    (1 << P_DELETE )
#define  S_FROM      (1 << P_FROM )
#define  S_WHERE     (1 << P_WHERE )
#define  S_FIELD     (1 << P_FIELD )
#define  S_STAR      (1 << P_STAR )
#define  S_COMMA     (1 << P_COMMA )
#define  S_LPARENTH  (1 << P_LPARENTH )
#define  S_RPARENTH  (1 << P_RPARENTH )
#define  S_LBRACKS   (1 << P_LBRACKS )
#define  S_RBRACKS   (1 << P_RBRACKS )
#define  S_AND       (1 << P_AND )
#define  S_OR 	     (1 << P_OR )
#define  S_EQUAL     (1 << P_EQUAL )
#define  S_LEQ	     (1 << P_LEQ )
#define  S_GEQ 	     (1 << P_GEQ )
#define  S_LESS      (1 << P_LESS )
#define  S_GREATER   (1 << P_GREATER )
#define  S_ADD 	     (1 << P_ADD )	
#define  S_DEL	     (1 << P_DEL )	
#define  S_ORDER     (1 << P_ORDER )
#define  S_BY	     (1 << P_BY )	
#define  S_TABNAME	 (1 << P_TABNAME )
#define  S_UPDATE    (1 << P_UPDATE)	
#define  S_INSERT    (1 << P_INSERT)
#define  S_CONSTANT  (1 << p_CONSTANT)
#define  S_TYPE      (1 << P_TYPE)
#define  S_VARIABLE  (1 << P_VARIABLE)
#define  S_ENDCHAR   (1 << P_ENDCHAR)


/**************** MASKS FOR DML****************/

#define  M_CREATE_DML    0
#define  M_DROP_DML      0
#define  M_TABLE_DML     0
#define  M_SELECT_DML    0|S_STAR|S_FIELD
#define  M_DELETE_DML    0|S_STAR|S_FROM
#define  M_FROM_DML      0|S_TABNAME
#define  M_WHERE_DML     0|S_FIELD|S_CONSTANT
#define  M_FIELD_DML     0|S_COMMA|S_FROM     
#define  M_STAR_DML      0|S_FROM
#define  M_COMMA_DML     0|S_FIELD
#define  M_LPARENTH_DML  0|S_SELECT
#define  M_RPARENTH_DML  0|S_COMMA
#define  M_LBRACKS_DML   0
#define  M_RBRACKS_DML   0
#define  M_AND_DML       0
#define  M_OR_DML 	     0
#define  M_EQUAL_DML     0
#define  M_LEQ_DML	     0
#define  M_GEQ_DML 	     0
#define  M_LESS_DML      0
#define  M_GREATER_DML   0
#define  M_ADD_DML 	     0
#define  M_DEL_DML	     0
#define  M_ORDER_DML     0
#define  M_BY_DML	     0
#define  M_TABNAME_DML	 0|S_WHERE|S_ENDCHAR
#define  M_UPDATE_DML    0
#define  M_INSERT_DML    0
#define  M_CONSTANT_DML  0
#define  M_TYPE_DML      0
#define  M_VARIABLE_DML  0
#define  M_ENDCHAR_DML   0
/******************* END DML *******************/


/***************MASKS FOR DDL*****************/
#define  M_CREATE_DDL    0|S_TABLE
#define  M_DROP_DDL      0|S_TABLE
#define  M_TABLE_DDL     0|S_TABNAME
#define  M_SELECT_DDL    0
#define  M_DELETE_DDL    0
#define  M_FROM_DDL      0
#define  M_WHERE_DDL     0
#define  M_FIELD_DDL     0|S_COMMA     
#define  M_STAR_DDL      0
#define  M_COMMA_DDL     0|S_TYPE
#define  M_LPARENTH_DDL  0
#define  M_RPARENTH_DDL  0
#define  M_LBRACKS_DDL   0|S_TYPE
#define  M_RBRACKS_DDL   0|S_ENDCHAR
#define  M_AND_DDL       0
#define  M_OR_DDL 	     0
#define  M_EQUAL_DDL     0
#define  M_LEQ_DDL	     0
#define  M_GEQ_DDL 	     0
#define  M_LESS_DDL      0
#define  M_GREATER_DDL   0
#define  M_ADD_DDL 	     0
#define  M_DEL_DDL	     0
#define  M_ORDER_DDL     0
#define  M_BY_DDL	     0
#define  M_TABNAME_DDL	 0|S_LBRACKS
#define  M_UPDATE_DDL    0
#define  M_INSERT_DDL    0
#define  M_CONSTANT_DDL  0
#define  M_TYPE_DDL      0
#define  M_VARIABLE_DDL  0|S_COMMA|S_RBRACKS
#define  M_ENDCHAR_DDL   0
/****************END DDL****************/

#define  OK      0
#define  EFIELD  1
#define  ENULL   2


#endif
