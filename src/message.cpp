#include"iostream"
#include"../include/message.hpp"
#include<thread>
#include<unistd.h>

Message::Message()
{

}

Message::Message(int outputType, int inputType)
{
  SetMessageTypes(outputType, inputType);
}

Message::~Message()
{

}

bool Message::InitMsg()
{
	key_t key = ftok(FILE_KEY, 0);
  CHECK("ftok", key);
  std::cout << "key " << key << std::endl;

	msgid_ = msgget(key, PERMISSION | IPC_CREAT | IPC_EXCL);
	if(msgid_ < 0)
	{
		if(errno == EEXIST)
		{
			msgid_ = msgget(key, PERMISSION);
			CHECK("msgget", msgid_);
		}
		else
		{
      perror("msgget");
			return false;
		}
	}
}

bool Message::StartReceiveLoop()
{
  std::thread receive (&Message::ReceiveLoop, this);

  receive.join();

  std::cout << "join success\n";

  return true;
}

bool Message::ReceiveLoop()
{
  MessageInputBuf buf;
  buf.type = inputType_;

  std::cout << "receive loop is started" << std::endl;

	do
  {
	  int length = msgrcv(msgid_, (struct msg_buf *)&buf, sizeof(InputData), 0, 0);
		CHECK("msgrcv", length);

    std::cout << "state " << buf.data.state << std::endl;
	} while(1);
}

bool Message::SetMessageTypes(int outputType, int inputType)
{
  outputType_ = outputType;
  inputType_ = inputType;
}

bool Message::SendMessage(MessageOutputBuf &buf)
{
  buf.type = outputType_;

  std::cout << "type " << buf.type << std::endl;
  int result = msgsnd(msgid_, (struct msgbuf *)&buf, sizeof(OutputData), 0);
  CHECK("msgsnd", result);
}


