/**
 * Copyright (c) 2010 Aldebaran Robotics
 */

#include <signal.h>

#define HELLOWORLD_IS_REMOTE

#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>

#include "StrategyModule.h"

#ifdef HELLOWORLD_IS_REMOTE
#   define ALCALL
#else
// when not remote, we're in a dll, so export the entry point
#   define ALCALL
#endif

extern "C"
{
ALCALL int _createModule(std::shared_ptr<AL::ALBroker> pBroker)
{
    // init broker with the main broker instance
    // from the parent executable
    AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
    AL::ALBrokerManager::getInstance()->addBroker(pBroker);
    // create module instances
    AL::ALModule::createModule<StrategyModule>(pBroker, "StrategyModule");
    return 0;
}

ALCALL int _closeModule(  )
{
    return 0;
}
} // extern "C"


#ifdef HELLOWORLD_IS_REMOTE
int main(int argc, char *argv[])
  {
    // pointer to createModule
    TMainType sig;
    sig = &_createModule;
    // call main
    return ALTools::mainFunction("StrategyModule", argc, argv, sig);
  }
#endif