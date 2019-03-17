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
    const std::string phraseToSay("Hello world");
    ALTextToSpeechProxy tts(getParentBroker());
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        tts.say(phraseToSay);
    }
}

bool StrategyModule::UpdateGameState(int state) {
    currentGameState = gamecontroller::GameState(state);
    if (currentGameState == gamecontroller::GameState::FINISHED) {
        is_terminated_.store(true);
        return true;
    }
    if (currentGameState == gamecontroller::GameState::PLAYING) {

        std::thread main_thread([&](){
            StartExecuting();
        });

        //https://en.cppreference.com/w/cpp/thread/thread/detach
        main_thread.detach();
        return true;
    }
}