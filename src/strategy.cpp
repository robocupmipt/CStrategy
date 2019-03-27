#include "../include/StrategyModule.h"

#include <iostream>
#include <alcommon/albroker.h>
#include <qi/log.hpp>
#include <alproxies/altexttospeechproxy.h>

using namespace AL;

StrategyModule::StrategyModule(boost::shared_ptr<ALBroker> broker, const std::string& name): ALModule(broker, name),
                                                                                           tts(getParentBroker())
{
  setModuleDescription("A tiny Strategy Module");

  functionName("UpdateGameState", getName(), "UpdateGameState");
  addParam("state", "The state from GameController");
  BIND_METHOD(StrategyModule::UpdateGameState);
}

void StrategyModule::init()
{
  std::cout << "init\n";

  startMovementTest();
}

StrategyModule::~StrategyModule() {}

void StrategyModule::StartExecuting()
{
    while(is_started_.load())
    {
      if(is_terminated_.load() == true)
      {
        is_started_.store(false);
        is_terminated_.store(false);

        std::cout << "finish\n";
      }
      else
      {
        std::cout << "state " << (int)currentGameState << '\n';

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }
}

void StrategyModule::UpdateGameState(int state) {
    ALTextToSpeechProxy tts(getParentBroker());

    currentGameState = gamecontroller::GameState(state);

    sayState(currentGameState);

    if((currentGameState == gamecontroller::GameState::FINISHED) && (is_started_.load() == true))
    {
      is_terminated_.store(true);
    }

    if (is_started_.load() == false) {
        is_started_.store(true);

        std::cout << "start\n";

        std::thread main_thread([&](){
            StartExecuting();
        });

        //https://en.cppreference.com/w/cpp/thread/thread/detach
        main_thread.detach();
    }
}

void StrategyModule::sayState(gamecontroller::GameState state)
{
    switch(state)
    {
      case 0:
        tts.say("initial");
        break;
      case 1:
        tts.say("ready");
        break;
      case 2:
        tts.say("set");
        break;
      case 3:
        tts.say("playing");
        break;
      case 4:
        tts.say("finished");
        break;
    }
}

void StrategyModule::startMovementTest()
{
  std::string command;

  boost::shared_ptr<AL::ALBroker> brokerTest;
  brokerTest = AL::ALBroker::createBroker("test", "0.0.0.0", 54000, "192.168.1.60", 9559);
  AL::ALProxy proxyMovement(brokerTest, "MovementGraph");

  do
  {
    std::cin >> command;

    if(command == "GoForward")
    {
      std::cout << "GoForward\n";
      proxyMovement.callVoid("GoForward");
    }

    else if(command == "GoBack")
    {
      std::cout << "GoBack\n";
      proxyMovement.callVoid("GoBack");
    }

    else if(command == "GoRight")
    {
      std::cout << "GoRight\n";
      proxyMovement.callVoid("GoRight");
    }

    else if(command == "GoLeft")
    {
      std::cout << "GoLeft\n";
      proxyMovement.callVoid("GoLeft");
    }

    else if(command == "Rotate")
    {
      std::cout << "Rotate (float)\n";

      float theta;
      std::cout << "theta = ";
      std::cin >> theta;
      std::cout << "theta = " << theta << '\n';

      proxyMovement.callVoid("Rotate", theta);
    }

    else if(command == "StartMove")
    {
      std::cout << "StartMove\n";
      proxyMovement.callVoid("StartMove");
    }

    else if(command == "StopMove")
    {
      std::cout << "StopMove\n";
      proxyMovement.callVoid("StopMove");
    }

    else if(command == "GetHeadVerticalAngle")
    {
      std::cout << "GetHeadVerticalAngle\n";

      float angle = proxyMovement.call<float>("GetHeadVerticalAngle");
      std::cout << "angle = " << angle << '\n';
    }

    else if(command == "GetHeadHorizontalAngle")
    {
      std::cout << "GetHeadHorizontalAngle\n";

      float angle = proxyMovement.call<float>("GetHeadHorizontalAngle");
      std::cout << "angle = " << angle << '\n';
    }

    else if(command == "SetHeadVerticalAngle")
    {
      std::cout << "SetHeadVerticalAngle (float)\n";

      float theta;
      std::cout << "theta = ";
      std::cin >> theta;
      std::cout << "theta = " << theta << '\n';

      proxyMovement.callVoid("SetHeadHorizontalAngle", theta);
    }

    else if(command == "SetHeadHorizontalAngle")
    {
      std::cout << "SetHeadHorizontalAngle (float)\n";

      float theta;
      std::cout << "theta = ";
      std::cin >> theta;
      std::cout << "theta = " << theta << '\n';

      proxyMovement.callVoid("SetHeadHorizontalAngle", theta);
    }

    else if(command == "LookDown")
    {
      std::cout << "LookDown (int: 0<=x<=7)\n";

      int x;
      std::cout << "x = ";
      std::cin >> x;
      std::cout << "x = " << x << '\n';

      proxyMovement.callVoid("LookDown", x);
    }

    else if(command == "ToInit")
    {
      std::cout << "ToInit\n";
      proxyMovement.callVoid("ToInit");
    }

    else if(command == "GetUpFront")
    {
      std::cout << "GetUpFront\n";
      proxyMovement.callVoid("GetUpFront");
    }

    else if(command == "GetUpBack")
    {
      std::cout << "GetUpBack\n";
      proxyMovement.callVoid("GetUpBack");
    }
    else
    {
      std::cout << "HELP\n";

      std::cout << "GoForward\n";
      std::cout << "GoBack\n";
      std::cout << "GoRight\n";
      std::cout << "GoLeft\n";
      std::cout << "Rotate (float)\n";
      std::cout << "StartMove\n";
      std::cout << "StopMove\n";
      std::cout << "GetHeadVerticalAngle\n";
      std::cout << "GetHeadHorizontalAngle\n";
      std::cout << "SetHeadVerticalAngle (float)\n";
      std::cout << "SetHeadHorizontalAngle (float)\n";
      std::cout << "LookDown (int: 0<=x<=7)\n";
      std::cout << "ToInit\n";
      std::cout << "GetUpFront\n";
      std::cout << "GetUpBack\n";
    }
  }
  while(command != "exit");
}


