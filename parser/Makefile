
OBJS = file_op.o \
       parser.o	 \
	   nodes.o 	 

LIB_OBJS = strings.o list.o bitops.o prints.o spinlocks.o atomic_ops.o


GCC += ${flags}

libs_path_t := $(foreach var, $(LIB_OBJS), $(libs_path)/$(var))


all:  ${OBJS}
	${GCC}  -o bparser ${OBJS}   $(libs_path_t)
	mv *.o Build

%.o: %.c 
	${GCC} -c  $< -o $@

clean:
	(cd Build/ && rm *.o )
	
