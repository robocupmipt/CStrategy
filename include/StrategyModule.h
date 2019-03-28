//
// Created by Nikita Mikhaylov on 2019-03-17.
//

#ifndef CSTRATEGY_STRATEGYMODULE_H
#define CSTRATEGY_STRATEGYMODULE_H
#include <alcommon/almodule.h>
#include <alproxies/altexttospeechproxy.h>
#include <alcommon/albroker.h>
#include <alproxies/almemoryproxy.h>
#include <alcommon/alproxy.h>
#include <alvalue/alvalue.h>

#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

#include "GameController.h"
#include "ComputerVisionAdapter.h"
#include "MovementGraphAdapter.h"
#include "LocalizationModuleAdapter.h"

using namespace AL;

namespace AL
{
    class ALBroker;
}

class StrategyModule : public AL::ALModule
{
public:
    StrategyModule(boost::shared_ptr<AL::ALBroker> pBroker, const std::string& pName);
    virtual ~StrategyModule();
    virtual void init(); //to call from GameController

    void UpdateBallPositions(Point2D element);

private:

    /* Tools for GameController Event */
    AL::ALMemoryProxy fMemoryProxy;
    void UpdateGameState(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg);

    //TEST TOOLS
    /* Proxy movement test */
    void StartMovementTest();
    /* For control robot states when module is local */
    void SayState(gamecontroller::GameState state);
    //TEST TOOLS

    /* Start */
    void StartExecuting();

    /* Loop to control other modules */
    void ExecutingLoop();

    /* Function to stop other modules when finished */
    void CompleteExecuting();

    void RotateHeadToFindBall();

private:
    /* Atomics for work with GameController */
    std::atomic<bool> is_terminated_{false};
    std::atomic<bool> is_started_{false};
    std::atomic<gamecontroller::GameState> currentGameState;

    AL::ALTextToSpeechProxy tts_;
    MovementGraphAdapter movement_graph_adapter_;
    LocalizationModuleAdapter localization_module_adapter_;
    ComputerVisionAdapter computer_vision_adapter_;

    std::deque<Point2D> balls_positions_;
    const int max_deque_size = 10;
};


#endif //CSTRATEGY_STRATEGYMODULE_H
