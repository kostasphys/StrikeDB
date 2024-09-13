OBJS= gen_types_head.o

#GCC += ${flags}

all: ${OBJS}


%.o: %.c 
	${GCC}  $< -o $@
	mv *.o Build/


clean:
	(cd Build/ && rm *.o)
	