#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inc/net/tcp.h>
#include <inc/messages.h>
#include <inc/debug.h>
#include <errno.h>

#define CHECK_READ  1;

#define max_val(x,y) (((x) >= (y)) ? (x) : (y))

int InitListen(int port)
{
    int ret, opt;
    struct sockaddr_in localSock;
    unsigned int localSock_len = sizeof(struct sockaddr_in);

    if ( (listenerFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      fprintf(stderr ," Error while creating listening socket \n "); 
      return -1;
   }

   opt = 1;

   localSock.sin_family          = AF_INET;
   localSock.sin_addr.s_addr     = htonl(INADDR_ANY);
   localSock.sin_port            = htons(port);

   if( setsockopt(listenerFd, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt)) != 0)
   {
      fprintf(stderr, "Problem when setting the sock opts \n;");
      close(listenerFd);
      return -1;
   }


   if ( bind(listenerFd, (struct sockaddr *)&localSock, sizeof(struct sockaddr_in)) == -1 )
   {
      fprintf(stderr, "Error while binding Listen socket \n");
      close(listenerFd);
      return -1;
   }

   ret = listen(listenerFd, 10);
   if( ret <= -1)
   {
      fprintf(stderr, "Error while listening \n");
      return ret;
   }

   fprintf(stderr ,"Listen socket is up \n\n");

   return 0;

}


int acceptConnect()
{
   struct sockaddr_in address_client;
   char address_client_dot[16];
   char msgLine[256];
   int clientFd;
   int addr_len = sizeof(struct sockaddr);

   sprintf(msgLine, "Start accepting connections");
   trace_file(msgLine);

   clientFd = accept(listenerFd, 
                     (struct sockaddr *)&address_client,
                     &addr_len);

   if( clientFd < 0 )
   {
      fprintf(stderr ,"Error during accepting call \n");
      return -1;
   }

   inet_ntop( AF_INET, &address_client.sin_addr.s_addr, address_client_dot, INET_ADDRSTRLEN );

   sprintf(msgLine, "Conntection accepted at adress %s \n\n", address_client_dot);
   trace_file(msgLine);
   
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
 


int writeMsgFast(int fd, struct line_packet *packet, int *writeBytes)
{
   char msgLine[255];
   ssize_t rem, count;
   void *writePtr = (void*)packet;
   rem =  sizeof(struct header_line) + packet->head.size;
   //rem =  sizeof(struct header_line);

   if(*writeBytes > 0)
   {
      writePtr += *writeBytes;
      rem = sizeof(struct header_line) + packet->head.size - *writeBytes;
   }
   

   if(packet->head.size > sizeof(struct line_msg))
   {
      sprintf(msgLine, "Size inside head is bigger than the body \n");
      trace_file(msgLine);
      return -1;
   }

   //memcpy(packet->buffer.buffer, buffer, packet.head.size );
   
   for(;;)
   {
      count = write(fd, writePtr, rem);

      if(count == 0 )
      {
         sprintf(msgLine, "Sockets closed normally or abruptly \n");
         trace_file(msgLine);
         return 0;
      } 
      else if(count < 0 )
      {  
         if( errno == EAGAIN || errno == EINTR)
         {
            sprintf(msgLine, "Socket is blocked now \n");
            trace_file(msgLine);  
            return -EAGAIN;
         }
         else
            return -1;
         
      }

      
      rem -= count;
      if( rem == 0 )
         break;
         
      //rem = rem - count;
      writePtr += count;
      *writeBytes += count;

     
   }
   
   sprintf(msgLine, "Its fdfdfdfd here\n");
   trace_file(msgLine);  


   memset(writeBytes, '\0', sizeof(*writeBytes));

   return 1;

}


int ReadMsgHead(int fd, struct line_packet *packet, int *readBytes)
{
      void *head_ptr = &packet->head;
   ssize_t ret, count;
   size_t  head_len, remain;
   //int readBytes = packet->rwBytes;
   char msgLine[256];

   head_len = sizeof(struct header_line);

   remain = head_len;

   #if ENABLE_DEBUG == 1
      sprintf(msgLine, "Start recv message ReadMsgHead \n");
      trace_file(msgLine);
   #endif
   

   if(*readBytes > 0)
   {
      head_ptr += *readBytes;
      remain = head_len - *readBytes; 
   }
   

   do
   {
           
     ret =  read(fd, head_ptr, remain);
     //ret =  recv(fd, head_ptr, remain, 0);
     if( ret <= 0)
     {
         if ( ret < 0  && (errno == EAGAIN || errno == EINTR))
         {
               /*The data is probably not ready yet so we will try again later*/
            sprintf(msgLine, "****RETRY AGAIN HEADER TCP****, fd:%d, ret:%d \n", fd, (int)ret);
            trace_file(msgLine);

            return -EAGAIN;
         }
         else
         {
            /*Normal close or Abrupt close. Either way we have to take a better look at this*/
            perror("");
            sprintf(msgLine, "Error during reading socket header. ret: %d, errno :%d  \n\n", (int)ret, errno);
            trace_file(msgLine);

            return 0;

         }
     }
     
    *readBytes += ret;
   
     head_ptr += ret;
     remain -= ret;

   }
   while( *readBytes < head_len );
   

   #if ENABLE_DEBUG == 1
      sprintf(msgLine, "TCP ReadMsgHead: message size is %d \n", (int)packet->head.size);
      trace_file(msgLine);
   #endif


   return 1;
}


/*It is assumed that the socket is Non Blocking*/
int readMsgFast(int fd, struct line_packet *packet, int *pReadBytes, int skipHead)
{

   void *head_ptr = &packet->head;
   void *msg_ptr = &packet->buffer;
   ssize_t ret, count;
   size_t msg_size, head_len, body_len, remain;
   int readBytes = *pReadBytes;
   char msgLine[256];

   head_len = sizeof(struct header_line);
   body_len = sizeof(struct line_msg );

   ret = 0;

  // memset(head_ptr, '\0', head_len);
  // memset(msg_ptr, '\0',  sizeof(struct line_msg));
   sprintf(msgLine, "Start recv readMsgFast \n");
   trace_file(msgLine);

   if( skipHead && max_val(readBytes - head_len, 0) >= packet->head.size)
   {
      sprintf(msgLine, "THE SIZES ARE EQ %d, %d\n", (int) (readBytes - head_len),  (int)packet->head.size);
      trace_file(msgLine);
      return 1;
   }
   else
   {
      sprintf(msgLine, "Size is %d\n", (int)(readBytes - head_len));
      trace_file(msgLine);
   }
      

   if(readBytes >= head_len)
      goto Body;

   head_ptr += readBytes;
   remain = head_len - readBytes;

   do
   {
      
     sprintf(msgLine, "Reading TCP Header \n");
     trace_file(msgLine);
     
     ret =  read(fd, head_ptr, remain);
     //ret =  recv(fd, head_ptr, remain, 0);
     if( ret <= 0)
     {
         if ( ret < 0  && errno == EAGAIN || errno == EINTR)
         {
               /*The data is probably not ready yet so we will try again later*/
            sprintf(msgLine, "****RETRY AGAIN TCP****, fd:%d, ret:%d \n", fd, (int)ret);
            trace_file(msgLine);

            *pReadBytes = readBytes;
            return -EAGAIN;
         }
         else
         {
            /*Normal close or Abrupt close. Either way we have to take a better look at this*/
            *pReadBytes = readBytes;
            sprintf(msgLine, "Error during reading socket header. ret: %d, errno :%d  \n\n", (int)ret, errno);
            trace_file(msgLine);

            return 0;

         }
     }
     
     readBytes += ret;
     head_ptr += ret;
     remain -= ret;
   }
   while( readBytes < head_len );
   

Body:
   msg_size = packet->head.size;

   #if ENABLE_DEBUG == 1
      sprintf(msgLine, "TCP: message size is %d \n", (int)msg_size);
      trace_file(msgLine);
   #endif

   
   remain = msg_size + head_len - readBytes;
   msg_ptr += readBytes - head_len;
   do
   {  
      
      ret =  read(fd, msg_ptr, remain);
    //  ret =  recv(fd, msg_ptr, remain, 0);
      
      sprintf(msgLine, "TCP: reading body %d \n", (int)ret);
      trace_file(msgLine);
      
      if(ret <= 0)
      {
         if ( ret < 0  && (errno == EAGAIN || errno == EINTR))
         {
            /*The data is probably not ready yet so we will try again later*/
            *pReadBytes = readBytes;
            return -errno;
         }
         else
         {
            *pReadBytes = readBytes;
            perror("");
            sprintf(msgLine, "Error during reading socket body: %d  \n\n", (int)ret);
            trace_file(msgLine);
            return  0;         
         }
      }
      
      #if ENABLE_DEBUG == 1
         sprintf(msgLine, "Bytes read inside tcp loop : %d \n", (int)ret);
         trace_file(msgLine);
      #endif
   
      readBytes += ret;
      remain -= ret;
      msg_ptr += ret;
      

   } while ( readBytes < msg_size + head_len);
   
   *pReadBytes =readBytes;

   sprintf(msgLine, "End recv message \n\n");
   trace_file(msgLine);

   return 1;

}


int readMsg(int fd, struct line_msg *buffer)
{
   struct header_line header;
   void *head_ptr = &header;
   void *msg_ptr = buffer;
   ssize_t ret, count, ovl;
   size_t msg_size, head_len;
   char msgLine[256];

   head_len = sizeof(struct header_line);
   ret = 0; ovl = 0;

   memset(head_ptr, '\0', head_len);
   memset(msg_ptr, '\0',  sizeof(struct line_msg));

   sprintf(msgLine, "Start recv message \n");
   trace_file(msgLine);

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
 
   sprintf(msgLine, "Header arrived with message %d \n", (int)msg_size);
   trace_file(msgLine);
  
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

   sprintf(msgLine, "End  recv  message \n\n");
   trace_file(msgLine);

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