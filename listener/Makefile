OBJS= 	listen_init.o listen_main.o  listen_connection.o \
	  	listener_utils.o listen_polling.o \


LIB_OBJS= tcp.o message_queue.o atomic_ops.o debug.o

libs_path_t := $(foreach var, $(LIB_OBJS), $(libs_path)$(var))


all: ${OBJS}
	${GCC}  -o blistener ${OBJS} ${libs_path_t} -lexplain
	mv *.o Build

%.o: %.c 
	${GCC} -c  $< -o $@

clean:
	rm *.o || (cd Build && rm *.o)
