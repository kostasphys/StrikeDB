#ifndef   __s_tcp_h__
#define   __s_tcp_h__

#include <inc/messages.h>

extern int listenerFd;

int InitListen(int port);
int acceptConnect();
int writeMsg(int fd, struct line_msg *buffer);
int readMsg(int fd, struct line_msg *buffer);

#endif