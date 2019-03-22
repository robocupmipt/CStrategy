//
// Created by Nikita Mikhaylov on 2019-03-17.
//

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
    ALTextToSpeechProxy tts(getParentBroker());
    tts.say("start module");
    while (is_started_.load()) {
      if(is_terminated_.load() == true) {

        // stop all modules
        tts.say("finished");

        is_terminated_.store(false);
      }
        std::cout << "kek\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void StrategyModule::UpdateGameState(int state) {
    std::cout << "state = " << state << '\n';
    currentGameState = gamecontroller::GameState(state);

    if((currentGameState == gamecontroller::GameState::FINISHED) && (is_started_.load() == true))
    {
      is_started_.store(false);
      is_terminated_.store(true);
    }

    if (is_started_.load() == false) {
        is_started_.store(true);

        std::thread main_thread([&](){
            StartExecuting();
        });

        //https://en.cppreference.com/w/cpp/thread/thread/detach
        main_thread.detach();
    }

    return true;
}
