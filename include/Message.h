//
// Created by Vladislav Molodtsov on 2019-03-31.
//

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/* --------------------- Private classes -------------------- */

/*
 *
 * Please, don't change anything, if you are not sure what you are doing.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/msg.h>
#include<errno.h>
#include<stdio.h>

#include"Custom.h"

namespace message
{

#define PERMISSION 0777

#define MESSAGE_CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
    perror(nameFunction);\
    return false;   \
	}							\
	else							\
		printf("%s succeeded\n", nameFunction);		\
} while(0)							\

// struct of message, that uses in msgrcv, msgsnd (Linux calls)
template<typename T>
struct MessageType
{
  long type;

  T data;
};

// The main class
template<typename SendTemplate, typename ReceiveTemplate>
class Message
{
  public:
  Message(int sendType, int receiveType);

  bool InitMsg();
  bool SetMessageTypes(int sendType, int receiveType);

  bool SendMessage(SendTemplate &message);
  ReceiveTemplate ReceiveMessage();

  int msgid_;
  int sendType_ = 1;
  int receiveType_ = 2;
};

/*
 *
 * Realization of methods
 *
 */

template<typename SendTemplate, typename ReceiveTemplate>
Message<SendTemplate, ReceiveTemplate>::Message(int sendType, int receiveType)
{
  SetMessageTypes(sendType, receiveType);

}
template<typename SendTemplate, typename ReceiveTemplate>
bool Message<SendTemplate, ReceiveTemplate>::SetMessageTypes(int sendType, int receiveType)
{
  sendType_ = sendType;
  receiveType_ = receiveType;
}

template<typename SendTemplate, typename ReceiveTemplate>
bool Message<SendTemplate, ReceiveTemplate>::InitMsg()
{
	key_t key = ftok(FILE_KEY, 0);
  MESSAGE_CHECK("ftok", key);

  std::cout << "key " << key << std::endl;

	msgid_ = msgget(key, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(msgid_ < 0)
	{
		if(errno == EEXIST)
		{
			msgid_ = msgget(key, PERMISSION);
			MESSAGE_CHECK("msgget", msgid_);
		}
		else
		{
			perror("msgget");

			return false;
		}
	}
}

template<typename SendTemplate, typename ReceiveTemplate>
bool Message<SendTemplate, ReceiveTemplate>::SendMessage(SendTemplate & message)
{
  std::cout << "SendMessage" << std::endl;
  message.type = sendType_;

  std::cout << "type " << message.type << std::endl;
  std::cout << "size " << sizeof(SendTemplate) << std::endl;
  int result = msgsnd(msgid_, (struct msgbuf *)&message, sizeof(SendTemplate), 0);
  MESSAGE_CHECK("msgsnd", result);
}

template<typename SendTemplate, typename ReceiveTemplate>
ReceiveTemplate Message<SendTemplate, ReceiveTemplate>::ReceiveMessage()
{
  std::cout << "ReceiveMessage" << std::endl;
  ReceiveTemplate buf;

  std::cout << "size " << sizeof(ReceiveTemplate) << std::endl;

  int length = msgrcv(msgid_, (struct msg_buf *)&buf, sizeof(ReceiveTemplate), receiveType_, 0);
  if(length != sizeof(ReceiveTemplate))
  {
    perror("msgrcv");

    std::cout << "Real length = " << length << std::endl;
    std::cout << "Correct length = " << sizeof(ReceiveTemplate) << std::endl;
  }
  else
  {
      std::cout << "msgrcv succeed" << std::endl;
      
  }

  std::cout << "type " << buf.type << std::endl;

  return buf;
}

} /* namespace message */

#endif /* __MESSAGE_H__ */
