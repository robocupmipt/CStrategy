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
#include"../include/message.hpp"

#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include "GameController.h"

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

    virtual void init();

private:

    /* Tools for GameController Event */
    AL::ALMemoryProxy fMemoryProxy;
    void UpdateGameState(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg);

    /* Start */
    void StartExecuting();

/* --------------------------------------- */
/* ---------------- DO IT ---------------- */
/* --------------------------------------- */
    /* Loop to control other modules */
    void ExecutingLoop();

    /* Function to stop other modules when finished */
    void CompleteExecuting();
/* --------------------------------------- */
/* ---------------- DO IT ---------------- */
/* --------------------------------------- */

    /* Proxy movement test */
    void StartMovementTest();

    /* Atomics for work with GameController */
    std::atomic<bool> is_terminated_{false};
    std::atomic<bool> is_started_{false};
    std::atomic<gamecontroller::GameState> currentGameState;

    /* For control robot states when module is local */
    void SayState(gamecontroller::GameState state);
    AL::ALTextToSpeechProxy tts_;
    Message message_;

/* ----------------- TRASH ----------------------- */

    struct ProAngle {

        ProAngle(double l, double r, double s) :
                left_(l),
                right_(r),
                step_(s) {}

        double left_;
        double right_;
        double step_;
    };

    //переписать на прокси
    //LocalizationModule localizationModule{};
    //MovementGraph movementGraph{};

/* ----------------- TRASH ----------------------- */
};

/* ------- ---- NEW PORTION OF TRASH -------------- */

namespace strategy {
    struct Command {
        enum CommandName {
            MoveForward,
            TurnLeft,
            TurnRight
        };

        CommandName name_;
        double angle_;
        double length_;
    };

    struct Point {
        double x;
        double y; //from gate to gate
    };
} //namespace strategy


//удалить
class LocalizationModule {
public:
    std::vector<strategy::Point> GetCurrentPositions() {
        //pass
    }
};


//удалить
class MovementGraph {
public:
    bool Move(float x, float y, float theta) {
        return false;
    }

    void SetTheta(const double d) {
        return;
    }

    void GoLeft(double smth) {
        return;
    }
};

/* ------- ---- NEW PORTION OF TRASH -------------- */

#endif //CSTRATEGY_STRATEGYMODULE_H
