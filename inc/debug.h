#ifndef     __s_debug_h__
#define     __s_debug_h__



/*
    0: Deactivated
    1: Activated
*/
#define ENABLE_DEBUG  1

extern int  trace_fd;

void debug_mesg(char *str);
void trace_file(char *msg);
int open_trace();

#endif