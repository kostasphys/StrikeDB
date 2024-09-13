#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inc/file_op.h>



struct fread_info *open_sql(const char *path){
	
	int fd; 
	size_t  size = 512;
	ssize_t	rcount, rcount_tot; 
	char file_buffer[1024];
	char *pstr, *mptr;
	struct fread_info *fi;
	
	fd = open(path, O_RDONLY);
	if(fd < 0){
		printf("Error opening sql file, err: %d", fd);
		exit(-1);
	}	
	
	rcount = 0;
	rcount_tot = 0;
	pstr = file_buffer;

	do{
	    rcount = read(fd, pstr, size);
		rcount_tot += rcount;
		pstr = file_buffer + rcount_tot%1024;
		
	} while (rcount<=0);
	
	if( rcount < 0  ){
		printf("Error reading sql file err: %zd", rcount);
		close(fd);
		exit(-1);
	} else {
		if(rcount_tot > 1024 )
		{
			printf("buffer overflow, file too big, count: %zd", rcount_tot);
			exit(-1);

		}
	}
	
	close(fd);	
	
	fi = (struct fread_info *) malloc(sizeof(struct fread_info));
		

	memcpy(fi->buff, file_buffer, rcount_tot );
	fi->size = rcount_tot;

	fi->buff[rcount_tot] = '\0';

	return fi;
	
}
