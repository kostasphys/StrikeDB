OBJS= exec_main.o exec_init.o  exec_threads_sync.o \
	  exec_threads.o exec_processing.o exec_ctrl.o

LIB_OBJS= message_queue.o

libs_path_t := $(foreach var, $(LIB_OBJS), $(libs_path)/$(var))

all: ${OBJS}
	${GCC}  -o bexec ${OBJS} ${libs_path_t}
	mv *.o Build

%.o: %.c 
	${GCC} -c  $< -o $@

cleano:
	rm *.o
