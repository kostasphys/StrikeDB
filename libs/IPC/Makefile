OBJS= message_queue.o

all: ${OBJS}

%.o: %.c 
	${GCC} -c  $< -o $@
	mv *.o ../Build/

#clean:
#	(cd ../Build/ && rm *.o )
