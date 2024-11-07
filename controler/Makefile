OBJS=control_thread.o

LIB_OBJS=  tcp.o debug.o atomic_ops.o

libs_path_t := $(foreach var, $(LIB_OBJS), $(libs_path)/$(var))


all: ${OBJS}
	${GCC}  -o bcontrol ${OBJS} ${libs_path_t}


%.o: %.c 
	${GCC} -c  $< -o $@

clean:
	rm bcontrol
	rm  *.o
