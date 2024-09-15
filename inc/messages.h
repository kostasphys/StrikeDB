#ifndef   __s_messages_h__
#define   __s_messages_h__

#include <mqueue.h>


struct sqlCommandMsgRx {
  int From;  
};

struct sqlCommandMsgTx {
  int From;  
  int cmd;
};


struct mqueue_msg{

  /*
    ID field basically indicates the recipient 
    of the message. Hardcoded.
  */
  unsigned long id; 

  unsigned long from; 
  
  /*
   * This is the id of the process that 
   * started the communication
   */
  mqd_t mq_id;

  /*Small buffer for comm*/
  char buff[256];

  unsigned long type;

  unsigned long status;

  unsigned long msg_code;

  unsigned long seq;
};


struct header_line
{
  size_t size;
} __attribute__((packed));

struct line_msg
{
  char buffer[128];
}__attribute__((packed));

struct line_packet
{
  struct header_line head;
  struct line_msg  buffer;
}__attribute__((packed));

#endif