#include "StrategyModule.h"

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
