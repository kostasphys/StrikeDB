OBJS= 	listener_init.o listener_main.o  listener_connection.o \
	  	listener_utils.o listener_polling.o  listener_auth.o \


LIB_OBJS= tcp.o message_queue.o atomic_ops.o debug.o list.o

libs_path_t := $(foreach var, $(LIB_OBJS), $(libs_path)$(var))


all: ${OBJS}
	${GCC}  -o blistener ${OBJS} ${libs_path_t} 
	mv *.o Build

%.o: %.c 
	${GCC} -c  $< -o $@

clean:
	rm  *.o   || true
	cd Build && rm *.o || true
