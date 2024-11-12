OBJS= strings.o  list.o \
	  bitops.o  debug.o \
	  atomic_ops.o


GCC += ${flags}

all: ${OBJS}


%.o: %.c 
	${GCC} -c  $< -o $@
	make -C net/
	make -C IPC/
	mv *.o Build/


clean:
	(cd Build/ && rm *.o)
	

