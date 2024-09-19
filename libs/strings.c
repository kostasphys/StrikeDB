#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inc/nodes_defs.h>



char *format_text(char *str)
{
	char *ptr, *temp;
	int length;

	length = strlen(str);

	ptr = malloc(2*length);
	if(!ptr)
	{
		printf("Error while allocating memory in format text \n");
		exit(-1);
	}


	temp = ptr;

	while(*str != '\0'){
		if(*str < 32)
		{
			*temp = 32;
			goto next;
		}
			

		if(*str == ';' || *str == ',' )
		{
			*temp = 32;
			++temp;
			*temp = *str; 
			++temp;
			*temp = 32;
			goto next;
		}
		
		*temp = *str;
next:
		++str;
		++temp;
	}

	*temp = '\0';

	return ptr;

}



int  trail_out_spaces(char **str){

	int len, i, j;
	char *ptr_end;


	len = strlen(*str);
	ptr_end = *str + len;



	for(;;){
		if( **str == 32 ){
			++*str;	
		}
		else 
			break;

	}

	if(*str == ptr_end)
		return 0;

	--ptr_end;

	for(;;){

		if(*ptr_end == 32)
			--ptr_end;
		else {
			++ptr_end;
			*ptr_end = '\0';
			return 0;
		}

	}

	return 0;
}

/*Don't put Empty strings in it*/
unsigned int keyword_to_hash(char *str)
{
	return (str[0]  + (str[0]*str[1]) )%(HASH_MAX - 1);
}



