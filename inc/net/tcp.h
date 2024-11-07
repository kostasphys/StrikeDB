#ifndef   __s_tcp_h__
#define   __s_tcp_h__

#include <inc/messages.h>

extern int listenerFd;

int InitListen(int port);
int acceptConnect();
int writeMsg(int fd, struct line_msg *buffer);
int writeMsgFast(int fd, struct line_packet *packet, int *writeBytes);
int readMsg(int fd, struct line_msg *buffer);
int readMsgFast(int fd, struct line_packet *packet, int *pReadBytes, int skipHead);
int ReadMsgHead(int fd, struct line_packet *packet, int *readBytes);

#endif