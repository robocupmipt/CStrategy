#ifndef MESSAGE_H
#define MESSAGE_H

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

#include<iostream>
#include"data.h"
#include<string>

#define CHECK(nameFunction, retValue)				\
do {								\
	if(retValue == -1)					\
	{							\
    perror(nameFunction);\
    return false;   \
	}							\
	else							\
		printf("%s succeeded\n", nameFunction);		\
} while(0)							\

#define FILE_KEY "../key"
#define PERMISSION 0777
#define FROM_COMMUNICATION_TO_STRATEGY 1
#define FROM_STRATEGY_TO_COMMUNICATION 2

struct InputData
{ enum GameState state;
};

struct OutputData
{
  enum GameState state;
};

struct MessageOutputBuf
{
	long type;  // It's necessary

  /* Next you can write your own data */

  OutputData data;
};

struct MessageInputBuf
{
	long type;  // It's necessary

  /* Next you can write your own data */

  InputData data;
};

class Message
{
  public:

  Message();
  Message(int outType, int inputType);

  ~Message();

  /* There are std::threads here */
  bool InitMsg();

  bool SendMessage(MessageOutputBuf &buf);

  bool SetMessageTypes(int output, int input);

  bool ReceiveLoop();

  bool StartReceiveLoop();

  private:

  int msgid_;

  int outputType_ = 1;
  int inputType_ = 2;
};

#endif // MESSAGE_H
