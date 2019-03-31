//
// Created by Nikita Mikhaylov and Vladislav Molodtsov on 2019-03-29.
//

#include <iostream>
#include <alcommon/albroker.h>
#include <qi/log.hpp>
#include <alproxies/altexttospeechproxy.h>

#include "../include/StrategyModule.h"

using namespace AL;

StrategyModule::StrategyModule(boost::shared_ptr<ALBroker> broker, const std::string& name): ALModule(broker, name), tts_(getParentBroker()), message_(FROM_STRATEGY_TO_COMMUNICATION, FROM_COMMUNICATION_TO_STRATEGY)
{
}

void StrategyModule::init()
{
  std::cout << "init\n";

  message_.InitMsg();

  std::thread receive([&](){
    ReceiveLoop();
  });

  receive.detach();

  //StartMovementTest();
}

StrategyModule::~StrategyModule()
{

}

void StrategyModule::ReceiveLoop()
{
  std::cout << "start receive loop\n";

  while(1)
  {
    MessageType<Receive> buf = message_.ReceiveMessage();

    UpdateGameState((gamecontroller::GameState)buf.data.state);
  }
}

void StrategyModule::ExecutingLoop()
{
  std::cout << "state " << (int)currentGameState << '\n';

  std::cout << "executing..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void StrategyModule::RotateHeadToFindBall()
{
    const double initial_horizontal_angle = -3;
    const double initial_vertical_angle =   -1;
    const double final_horizontal_angle =    3;
    const double final_vertical_angle =      1;
    const double horizontal_step =           1;
    const double vertical_step =            0.5;

    double horizontal_angle = initial_horizontal_angle;
    double vertical_angle = initial_vertical_angle;

    while (horizontal_angle <= final_horizontal_angle) {
        movement_graph_adapter_.SetHeadHorizontalAngle(horizontal_angle);
        while (vertical_angle <= final_vertical_angle) {
            movement_graph_adapter_.SetHeadVerticalAngle(vertical_angle);
            //time to detect ball in other thread in ComputerVisionModule
            std::this_thread::sleep_for (std::chrono::seconds(1));
            vertical_angle += vertical_step;
        }
        horizontal_angle += horizontal_step;
    }
}

void StrategyModule::CompleteExecuting()
{
    RotateHeadToFindBall();
    std::cout << "finish\n";
}

void StrategyModule::StartExecuting()
{
    while(is_started_.load())
    {
      if(is_terminated_.load() == true)
      {
        is_started_.store(false);
        is_terminated_.store(false);

        CompleteExecuting();
      }
      else
      {
        ExecutingLoop();
      }
    }
}

void StrategyModule::UpdateGameState(gamecontroller::GameState state)
{
    currentGameState = state;
    SayState(currentGameState);

    if((currentGameState == gamecontroller::GameState::FINISHED) && (is_started_.load() == true))
    {
      is_terminated_.store(true);
    }

    if (is_started_.load() == false)
    {
        is_started_.store(true);

        std::cout << "start\n";

        std::thread main_thread([&](){
            StartExecuting();
        });

        //https://en.cppreference.com/w/cpp/thread/thread/detach
        main_thread.detach();
    }
}

void StrategyModule::UpdateBallPositions(Point2D element)
{
    /*TODO: Write thread-safe code*/
    if (balls_positions_.size() >= max_deque_size) {
        balls_positions_.pop_front();
    }
    balls_positions_.push_back(element);
}

void StrategyModule::SayState(gamecontroller::GameState state)
{
    switch(state)
    {
      case gamecontroller::INITIAL:
        tts_.say("initial");
        break;
      case gamecontroller::READY:
        tts_.say("ready");
        break;
      case gamecontroller::SET:
        tts_.say("set");
        break;
      case gamecontroller::PLAYING:
        tts_.say("playing");
        break;
      case gamecontroller::FINISHED:
        tts_.say("finished");
        break;
      case gamecontroller::PENALIZED:
        tts_.say("penalized");
        break;
    }
}

/* Vlad will remove soon */
void StrategyModule::StartMovementTest()
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

      proxyMovement.callVoid("SetHeadVerticalAngle", theta);
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

