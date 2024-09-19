
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inc/strings.h>
#include <inc/nodes.h>
#include <sys/shm.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <inc/exec/exec_threads.h>


/*

/*

// All legal nodes are stored in this array. Each node determines which transition
// is legal and which is not. 

struct qnode nodes_array[P_MAXELM] = { 

		{"CREATE"  , P_CREATE  , {M_CREATE_DML, M_CREATE_DDL},  	2}, 
		{"DROP"    , P_DROP    , {M_DROP_DML, M_DROP_DDL},  		2},
		{"TABLE"   , P_TABLE   , {M_TABLE_DML, M_TABLE_DDL},		0},
		{"SELECT"  , P_SELECT  , {M_SELECT_DML, M_SELECT_DDL},		1},
		{"DELETE"  , P_DELETE  , {M_DELETE_DML, M_DELETE_DDL},		1},
		{"FROM"    , P_FROM    , {M_FROM_DML, M_FROM_DDL},			0},
		{"WHERE"   , P_WHERE   , {M_WHERE_DML, M_WHERE_DDL},		0},
		{""   	   , P_FIELD   , {M_FIELD_DML, M_FIELD_DDL},		0},
		{"*"  	   , P_STAR    , {M_STAR_DML, M_STAR_DDL},			0},			
		{","  	   , P_COMMA    , {M_COMMA_DML, M_COMMA_DDL},		0},
		{"("  	   , P_LPARENTH , {M_LPARENTH_DML, M_RPARENTH_DDL},	0},
		{")"  	   , P_RPARENTH , {M_LPARENTH_DML, M_RPARENTH_DDL},	0},
		{"{"  	   , P_LBRACKS  , {M_LBRACKS_DML, M_RBRACKS_DDL},	0},
		{"}"  	   , P_RBRACKS  , {M_LBRACKS_DML, M_RBRACKS_DDL},	0},
		{"AND"     , P_AND     , {M_AND_DML, M_AND_DDL},			0},
		{"OR"  	   , P_OR  	   , {M_OR_DML, M_OR_DDL},				0},
		{"="  	   , P_EQUAL   , {M_EQUAL_DML, M_ENDCHAR_DDL},		0},
		{"<="  	   , P_LEQ     , {M_LEQ_DML, M_LEQ_DDL},			0},
		{">="  	   , P_GEQ     , {M_GEQ_DML, M_GEQ_DDL},			0},
		{"<"  	   , P_LESS    , {M_LEQ_DML, M_LEQ_DDL},			0},
		{">"  	   , P_GREATER , {M_GREATER_DML, M_GREATER_DDL},	0},
		{"+"  	   , P_ADD     , {M_ADD_DML, M_ADD_DDL},			0},
		{"-"  	   , P_DEL     , {M_DEL_DML, M_DEL_DDL},			0},
		{"ORDER"   , P_ORDER   , {M_ORDER_DML, M_ORDER_DDL},		0},
		{"BY"      , P_BY      , {M_BY_DML, M_BY_DDL},				0},
		{""  	   , P_TABNAME , {M_TABLE_DML, M_TABLE_DDL},		0},
		{"UPDATE"  , P_UPDATE  , {M_UPDATE_DML, M_UPDATE_DDL},		1},
		{"INSERT"  , P_INSERT  , {M_INSERT_DML, M_INSERT_DDL},		1},
		{""  	   , p_CONSTANT , {M_CONSTANT_DML, M_CONSTANT_DDL}, 0},
		{""  	   , P_TYPE     , {M_TYPE_DML, M_TYPE_DDL},			0},
		{""  	   , P_VARIABLE , {M_VARIABLE_DML, M_VARIABLE_DDL},	0},
		{";"  	   , P_ENDCHAR , {M_ENDCHAR_DML, M_ENDCHAR_DDL},	0}
};

unsigned int keyword_to_hash(char *str)
{
	return (str[0]  + (str[0]*str[1]) )%(HASH_MAX - 1);
}


int hash_list_init(){
	int i, id;
	struct hash_node *ptr, *mptr;


	for(i=0; i<P_MAXELM; ++i)
	{
			if( ( strlen(nodes_array[i].name) == 0 ) )
				continue;

			
			id = keyword_to_hash(nodes_array[i].name);	

			
			if(hash_node_list[id].qptr != NULL)
			{
				ptr = &hash_node_list[id];
				while (ptr->next != NULL)
					ptr = ptr->next;

				mptr = malloc(sizeof(struct hash_node));

				if(!mptr){
					printf("Could not allocate memory for hash node table \n");
					return -1;
				}

				ptr->next = mptr;
				mptr->next = NULL;

				mptr->qptr = &nodes_array[i];
				
			} else {

				hash_node_list[id].qptr = &nodes_array[i];

			}

	}


	return 0;
	
}


struct hash_node hash_node_list[HASH_MAX];


void *shm;
int smid;


struct whatever{
	int ak;
	char  bff[30];
};

int main(){
	mqd_t mqd;

	mqd = mq_open("/novvel", O_RDWR);
	printf(" MQ ID :%d \n", mqd);
	int i = 0 ;

	struct whatever obj, buffer; 

	obj.ak = 123;
	strcpy(obj.bff,"Hi hello");

	
	while(1){
		int ff, i;
		ff = mq_send(mqd, &obj, sizeof(struct whatever), NULL);
		printf("Mm %d \n",ff);
		sleep(1);
//		i = mq_receive(mqd, &buffer, sizeof(struct whatever), NULL);
//		printf("Received : %s \n", buffer.bff);

		//break;
	}
/*	
	smid = shmget((key_t)0, 4096, 0666|IPC_CREAT);
	printf("Smid %d\n", smid);
*/
	//shmctl(smid, IPC_RMID, NULL);
	/*
	struct hash_node *ptr;

    hash_list_init();
	
	//ptr = &hash_node_list[0];

	for(int i=0;i<HASH_MAX; ++i) {
		if (hash_node_list[i].qptr == NULL)
			continue;
		
		ptr = &hash_node_list[i];

		do
		{
			printf("ID:%d,  %s || ",i ,ptr->qptr->name);
			ptr = ptr ->next;

		} while(ptr);

		printf("\n");

	}

} 
*/
static pthread_mutex_t syncWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  syncWriteCond  = PTHREAD_COND_INITIALIZER;


void *threadfn(void *arg)
{
	printf("Thread started \n");
//	sleep(5);
	pthread_cond_signal(&syncWriteCond);
	printf("Signal broadcasted \n\n");

}


int main()
{	
	pthread_t tid;
	 void **val;
	if ( pthread_create(&tid, NULL, threadfn, NULL) < 0 )
	{
		printf("Fak \n");
		exit(-1);
	}

	printf("We sleep for 10 secs \n");
	sleep(5);

	pthread_cond_wait(&syncWriteCond, &syncWriteMutex);

	printf("Condition broke \n");
	sleep(2);
	pthread_join(tid, NULL);
	printf("Fakakakaka \n");
	return 0;
}



