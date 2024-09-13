OBJS=control_thread.o

LIB_OBJS= message_queue.o

libs_path_t := $(foreach var, $(LIB_OBJS), $(libs_path)/$(var))


all: ${OBJS}
	${GCC}  -o bcontrol ${OBJS} ${libs_path_t}


%.o: %.c 
	${GCC} -c  $< -o $@
