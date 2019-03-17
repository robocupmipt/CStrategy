//
// Created by Nikita Mikhaylov on 2019-03-17.
//

#ifndef CSTRATEGY_STRATEGYMODULE_H
#define CSTRATEGY_STRATEGYMODULE_H

#include <alcommon/almodule.h>

#include <memory>
#include <string>

#include "KernelStrategy.h"

namespace AL
{
    class ALBroker;
}

class StrategyModule : public AL::ALModule
{
public:
    StrategyModule(std::shared_ptr<AL::ALBroker> pBroker, const std::string& pName);

    virtual ~StrategyModule();

    /* This is called right after the module has been loaded*/
    virtual void init() {};

    bool UpdateGameState(int);
private:
    KernelStrategy kernel_;
};

#endif //CSTRATEGY_STRATEGYMODULE_H
