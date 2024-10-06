#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <inc/debug.h>

#define CHAR_MAX   256
#define print_str(x) #x

int trace_fd;
int debug_trace = 1;
pthread_mutex_t  trace_mutex = PTHREAD_MUTEX_INITIALIZER;


void debug_mesg(char *str)
{

if( ENABLE_DEBUG == 1)
{   
        
        //printf("%s \n\n", str);
        printf(print_str(%s \n\n), str);
    
}

}

void trace_file(char *msg)
{
        char message_buffer[CHAR_MAX];
        int msg_len, write_len;
        int j;

        if(!debug_trace)
                return ;
        
        msg_len = strlen(msg);
        

        for(j=0; j < msg_len; j+=90)
        {
                sprintf(message_buffer, "|%08d|%08d| %.90s", getpid(), gettid(), msg + j );
                write_len = strlen(message_buffer);

                pthread_mutex_lock(&trace_mutex);

                write(trace_fd, message_buffer, write_len);

                pthread_mutex_unlock(&trace_mutex);
        }
        
}

int open_trace(int flags)
{
        char  trace_path[255];
        char *dir_path;
        int dir_len;
        const char *name = "HOME_DIR";

        dir_path = getenv(name);

        if(!trace_path)
        {
                printf("Environmental variable 'HOME_DIR' does not exist.  \n" );
                return -1;
        }

        
        strcpy(trace_path, dir_path);  
        dir_len = strlen(trace_path);

        strcpy(trace_path + dir_len, "logs/listener_trace");

        trace_fd = open(trace_path, O_WRONLY|O_APPEND|O_CREAT|flags, S_IRUSR|S_IWUSR);
        if( trace_fd < 0 )
        {
                printf("Error while opening trace file\n");
                perror("");
                return -1;
        }

        return 0;
}