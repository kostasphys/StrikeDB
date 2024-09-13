#ifndef    __s_file_op_h__
#define    __s_file_op_h__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


struct fread_info {
	char   buff[1024];
        ssize_t   size; 
};

extern struct fread_info *open_sql(const char *path);



#endif
