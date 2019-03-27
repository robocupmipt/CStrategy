//
// Created by Nikita Mikhaylov on 2019-03-17.
//

#ifndef CSTRATEGY_STRATEGYMODULE_H
#define CSTRATEGY_STRATEGYMODULE_H

#include <alcommon/almodule.h>
#include <alproxies/altexttospeechproxy.h>
#include <alcommon/albroker.h>

#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

#include "GameController.h"

using namespace AL;

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
    LocalizationModule localizationModule{};
    MovementGraph movementGraph{};

/* ----------------------------------------------------- */

    void sayState(gamecontroller::GameState state);
    void UpdateGameState(int);
    void StartExecuting();

    void startMovementTest();

    std::atomic<bool> is_terminated_{false};
    std::atomic<bool> is_started_{false};
    std::atomic<gamecontroller::GameState> currentGameState;

    AL::ALTextToSpeechProxy tts;

/* ----------------------------------------------------- */
};

#endif //CSTRATEGY_STRATEGYMODULE_H
