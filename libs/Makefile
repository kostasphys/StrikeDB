OBJS= strings.o  list.o \
	  bitops.o  debug.o \
	  atomic_ops.o


GCC += ${flags}

all: ${OBJS}


%.o: %.c 
	${GCC} -c  $< -o $@
	mv *.o Build/


clean:
	(cd Build/ && rm *.o)
	

