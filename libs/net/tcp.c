#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inc/net/tcp.h>
#include <inc/messages.h>
#include <errno.h>

#define CHECK_READ  1;

int InitListen(int port)
{
    int ret, opt;
    struct sockaddr_in localSock;
    unsigned int localSock_len = sizeof(struct sockaddr_in);

    if ( (listenerFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      printf(" Error while creating listening socket \n "); 
      return -1;
   }

   opt = 1;

   localSock.sin_family          = AF_INET;
   localSock.sin_addr.s_addr     = htonl(INADDR_ANY);
   localSock.sin_port            = htons(port);

   if( setsockopt(listenerFd, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt)) != 0)
   {
      printf("Problem when setting the sock opts \n;");
      close(listenerFd);
      return -1;
   }


   if ( bind(listenerFd, (struct sockaddr *)&localSock, sizeof(struct sockaddr_in)) == -1 )
   {
      printf("Error while binding Listen socket \n");
      close(listenerFd);
      return -1;
   }

   ret = listen(listenerFd, 10);
   if( ret <= -1)
   {
      printf("Error while listening \n");
      return ret;
   }

   printf("Listen socket is up \n\n");

   return 0;

}


int acceptConnect()
{
   struct sockaddr_in address_client;
   char address_client_dot[16];
   int clientFd;
   int addr_len = sizeof(struct sockaddr);

   printf("Start accepting connections \n");

   clientFd = accept(listenerFd, 
                     (struct sockaddr *)&address_client,
                     &addr_len);

   if( clientFd < 0 )
   {
      printf("Error during accepting call \n");
      return -1;
   }

   inet_ntop( AF_INET, &address_client.sin_addr.s_addr, address_client_dot, INET_ADDRSTRLEN );

   printf("Conntection accepted at adress %s \n\n", address_client_dot);
   
   return clientFd;
}



int writeMsg(int fd, struct line_msg *buffer)
{
   struct line_packet packet;
   ssize_t rem, count;
   rem =  sizeof(struct header_line) + sizeof(struct line_msg);

   packet.head.size = sizeof(struct line_msg);
   memcpy(&packet.buffer, buffer, packet.head.size );
   
   while(1)
   {
      count = write(fd, &packet, rem);

      if(count <= 0 )
      {
         printf("Error while writing to socket: %d\n\n", (int)count );
         return -1;
      }

      //perror("");
      printf("%d bytes where sent \n\n", (int)count);

      rem = rem - count;
      buffer = buffer + count;

      if( rem == 0 )
         break;
   }
   
   return 0;

}

/*It is assumed that the socket is Non Blocking*/
int readMsgFast(int fd, struct line_packet *packet, int *pReadBytes)
{

   void *head_ptr = &packet->head;
   void *msg_ptr = &packet->buffer;
   ssize_t ret, count;
   size_t msg_size, head_len, body_len, remain;
   int readBytes = *pReadBytes;

   head_len = sizeof(struct header_line);
   body_len = sizeof(struct line_msg );

   ret = 0;

  // memset(head_ptr, '\0', head_len);
  // memset(msg_ptr, '\0',  sizeof(struct line_msg));
   printf("Start recv message \n");

   if(readBytes >= head_len)
      goto Body;

   head_ptr += readBytes;
   remain = head_len - readBytes;

   do
   {
     printf("READING HEADER --- \n\n");
     ret =  read(fd, head_ptr, remain);
     if( ret <= 0)
     {
         if ( ret <= 0  && errno == 11)
         {
               /*The data is probably not ready yet so we will try again later*/
            printf("****RETRY AGAIN TCP**** \n\n");
            *pReadBytes = readBytes;
            return EAGAIN;
         }
         else
         {
            *pReadBytes = readBytes;
            printf("Error during reading socket header: %d  \n\n", (int)ret);  
            return -errno;

         }
     }
     
     readBytes += ret;
     head_ptr += ret;
     remain -= ret;
   }
   while( readBytes < head_len );
   

   msg_size = packet->head.size;
   printf("TCP: message size is %d \n", (int)msg_size);

Body:
   remain = msg_size + head_len - readBytes;
   msg_ptr += readBytes - head_len;
   do
   {  
      
      ret =  read(fd, msg_ptr, remain);
      printf("TCP: reading body %d \n", (int)ret );
      if(ret <= 0)
      {
         if ( ret <= 0  && errno == 11)
         {
            /*The data is probably not ready yet so we will try again later*/
            *pReadBytes = readBytes;
            return EAGAIN;
         }
         else
         {
            *pReadBytes = readBytes;
            printf("Error during reading socket body: %d  \n\n", (int)ret);  
            return -errno;         
         }
      }
      printf("Bytes read inside tcp loop : %d \n", (int)ret);
      readBytes += ret;
      msg_ptr += ret;
      remain -= ret;

   } while ( readBytes < msg_size + head_len);
   
   *pReadBytes =readBytes;
   printf("End  recv  message \n\n");

   return 0;

}

int readMsg(int fd, struct line_msg *buffer)
{
   struct header_line header;
   void *head_ptr = &header;
   void *msg_ptr = buffer;
   ssize_t ret, count, ovl;
   size_t msg_size, head_len;

   head_len = sizeof(struct header_line);
   ret = 0; ovl = 0;

   memset(head_ptr, '\0', head_len);
   memset(msg_ptr, '\0',  sizeof(struct line_msg));

   printf("Start recv message \n");

   do
   {
     ret =  read(fd, head_ptr, head_len);
     ovl += ret;
     head_ptr += ret;
   }
   while( ovl < head_len && ret > 0 );
   
   if (ret <= 0 )
   {
      printf("Error during reading socket header: %d  \n\n", (int)ret);     
      return -1;

   }

   msg_size = header.size;
   printf("Header arrived with message %d \n", (int)msg_size);

   ovl = 0;
   do
   {
     ret =  read(fd, msg_ptr, msg_size);
     ovl += ret;
     msg_ptr += ret;
   }
   while( ret > 0 && ovl < msg_size );

   if (ret <= 0 )
   {
      printf("Error during reading Message body: %d  \n\n", (int)ret);
      perror("");
      return -1;

   }

   printf("End  recv  message \n\n");

   return 0;

}

/*
int readPollClientSockets()
{
   
   FD_ZERO(&readfds);
   sfd = 0;

   ret = select(sfd, &readfds, NULL, NULL,NULL);
}

*/