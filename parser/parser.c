#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inc/file_op.h>
#include <inc/gentools.h>
#include <inc/parser.h>
#include <inc/strings.h>
#include <inc/nodes.h>
#include <inc/bitops.h>
#include <inc/prints.h>
#include <inc/compiler.h>
#include <inc/spinlocks.h>
#include <pthread.h>

static struct fread_info *sql_obj;
static char *sql_string;


int lockito;

/*
The head of the token list
*/
struct token token_head;

/*##### FUNCTION DECLARATIONS ##################*/
static void  preprocess_command(char * );
static int  create_token_list(char *sql);
static  int resolve_field_type(struct token *tk, struct token *prev, int mask);
static void check_token(struct token *tk, struct token *prev, int mask);
void debug_list();
/*##### FUNCTION DECLARATIONS ##################*/


int parse_main(char *qstr){;
	
	preprocess_command(qstr);
	//debug_list();
	create_token_list(sql_string);
		
}



static void  preprocess_command(char *path)
{

	sql_obj = open_sql(path);
	sql_string = sql_obj->buff;	

	sql_string = format_text(sql_string);	
	trail_out_spaces(&sql_string);


	if(sql_string == NULL){
		printf("Empty File, we exit.");
		exit(0);
	}
		
}


static struct token *generate_token(char *str, int *errcode)
{
	size_t size= 0;
	struct token *ptr; 
	char *temp = str;

	
	debug_mesg("Loop char:");
	while(  *temp != 32 && *temp != '\0'){
		
		printf("%c",*temp);
		++temp;
		++size;		
	}
	
	//printf("\n\n",*temp);

	
	ptr = malloc(sizeof(struct token));
	
	if(!ptr){
		printf("Error during malloc \n");
		return NULL;
	}

	memcpy(ptr->name, str, size );
	*(ptr->name + size) = '\0';

	ptr->name_size = (unsigned short)size;

	*errcode = find_qnode(ptr);

	debug_mesg("FIND QNODE");

	if( *errcode == -ENULL)
	{
		printf("ON ENULL \n");	
		return  ptr;
	}
	
	add_tail(&(token_head.token_list), &(ptr->token_list)); 

	debug_mesg("AFTER TAIL ");

	return ptr;
	
}

//##########################

static int  create_token_list(char *sql )
{
	int len, idx_mask;
	char cmd_type;
	struct token *first, *prev, *curr;
	int err;
	
	
	
	first = generate_token(sql, &err);
	if(!first){
		printf("Error in generate_token \n");
		exit(-1);
	}
	

	if( err == -ENULL)
	{
		printf("Null token, that's illegal \n");
		exit(-1);
	} 

	/*
	else if ( err == -EFIELD)
	{
		printf("The first token should not be a general field \n");
		exit(-1);
	}
	*/

	idx_mask = first->qnode->qstart;


	if(idx_mask > 0)
	{

		idx_mask -= 1;

		if( idx_mask > 1)
		{
			printf("Illegal mask id !!! \n");
			exit(-1);
		}
		
	}
	else
	{
		printf("Start keyword is illegal \n");
		exit(-1);

	}


	prev = first;
	sql += first->name_size;

	

	for(;;) {
	
		while(*sql == 32 || *sql == 9 || *sql == 10)
		{
			++sql;
		}
			
		
		if(*sql == '\0')
		{
			printf("We reached end of query \n");
			break;
		}
			

		debug_mesg("Before GENERATE TOKEN");

		curr = generate_token(sql, &err);

		debug_mesg("After GENERATE TOKEN");

		if( err == -EFIELD )
		{	
			int fak;
			fak  =  resolve_field_type(curr, prev, idx_mask);
			printf("RESOLVE ENDER!! %d \n",fak);

			if( fak == -1 )
			{
				printf("This token cannot be a field type \n");
				exit(-1);
			}

			goto cont;

		}


		check_token(curr, prev, idx_mask);


cont:
		sql += curr->name_size;
		prev = curr;
	
	} 
	
}

static  int resolve_field_type(struct token *tk, struct token *prev, int mask)
{
	//struct bitmap_node bm;
	debug_mesg("RESOLVE START");

	unsigned long *bmap = prev->qnode->bitmap_arr[mask].bitmap;

	unsigned int npos;
	npos = prev->qnode->shift_op;
	

	if(mask == 0)
	{
		//TODO LATER
		return 0;
	}
	else
	{

		switch(npos){
			case P_TABLE: 
			{
				tk->qnode = &nodes_array[P_TABNAME];
				break;
			}

			case P_TYPE:
			{
				tk->qnode = &nodes_array[P_VARIABLE];
				break;
			}

			case P_COMMA:
			{
				tk->qnode = &nodes_array[P_TYPE];
				break;
			}	

			case P_LBRACKS:
			{
				tk->qnode = &nodes_array[P_TYPE];
				break;
			}

			default:
			{
				printf("RESOLVE ERROR \n");
				return -1;
			}
		}

		debug_mesg(" RESOLVE END ");

		return 0;
		
	}
	

}

static void check_token(struct token *tk, struct token *prev, int mask)
{
	unsigned long *bmap;

	debug_mesg("Start check_token");
	//printf("BEfore BMAP %s \n", prev->qnode->name);

	bmap = prev->qnode->bitmap_arr[mask].bitmap;

	//printf("Bitmap : %lu \n", bmap[0]);
	

	if( test_bit_abs(bmap, (unsigned int)tk->qnode->shift_op) == 0)
	{
		printf("Illegal transition at %s \n",tk->name);
		exit(-1); 
	}

	debug_mesg("END check token");

}



static int dd_token_list(){
	return 0;
};



void  debug_list(){
	
	struct list_head *ptr = &token_head.token_list;	
	struct token *tp;
	
	debug_mesg("------DEBUG START------");
	
	for(;;){
				
	
		ptr = ptr->next;


		printf("TOKEN HEAD IN DEBUG %p, %p, %p \n",&token_head.token_list ,token_head.token_list.prev, token_head.token_list.next );
		if(ptr == &token_head.token_list)
			break;


		tp = container_of(ptr, struct token, token_list);
		printf("[name=%s, size=%d  ]",tp->name, tp->name_size);
		printf("%s  ",tp->name);
	}
	
	debug_mesg("------DEBUG END------");
}

void *thread_example(void *ptr)
{

	int count2;

	count2 = *( (int*)ptr );

	unsigned long  i = 0;

	while( i < 10000000)
	{
		//atomic_inc(&lockito);
		++i;
	}

	return NULL;
}




int main(){
	
	
	debug_mesg("---------START MAIN---------");

	pthread_t pid1;
	int countt;



	if (pthread_create( &pid1, NULL, thread_example, (void* )&countt ) < 0)
   	{
      printf("ada\n");
      exit(0);
    }

	pthread_join(pid1, NULL);

	INIT_HEAD((struct list_head*)&token_head.token_list);
	
	hash_list_init();
	//printf("TOKEN HEAD %p, %p, %p \n",&token_head.token_list ,token_head.token_list.prev, token_head.token_list.next );

	parse_main("./test_folder/sqlfile");

	return 0;
}







