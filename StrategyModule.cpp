//
// Created by Nikita Mikhaylov on 2019-03-17.
//

#include "StrategyModule.h"

#include <iostream>
#include <alcommon/albroker.h>
#include <qi/log.hpp>
#include <alproxies/altexttospeechproxy.h>

using namespace AL;

StrategyModule::StrategyModule(std::shared_ptr<ALBroker> broker, const std::string& name):
        ALModule(broker, name)
{
  setModuleDescription("A tiny Strategy Module");

  functionName("UpdateGameState", UpdateGameState(), "UpdateGameState");
  addParam("state", "The state from GameController");
  BIND_METHOD(StrategyModule::UpdateGameState());
}

StrategyModule::~StrategyModule() {}

bool StrategyModule::UpdateGameState(int state) {
  return kernel_.updateGameState(state);
}

