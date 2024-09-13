#include <stdio.h>
#include <stdarg.h>
#include <inc/prints.h>


#define print_str(x) #x




void debug_mesg(char *str){

if( ENABLE_DEBUG == 1)
{   
        
        //printf("%s \n\n", str);
        printf(print_str(%s \n\n), str);
    
}

}