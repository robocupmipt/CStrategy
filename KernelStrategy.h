#ifndef CSTRATEGY_KERNELSTRATEGY_H
#define CSTRATEGY_KERNELSTRATEGY_H

#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

#include "GameController.h"

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



class KernelStrategy {
public:
    bool updateGameState(int);
private:
    void startExecuting();

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

    std::atomic<bool> is_terminated_{false};
    std::atomic<bool> is_paused_{false};

    std::atomic<gamecontroller::GameState> currentGameState;
};


#endif //CSTRATEGY_KERNELSTRATEGY_H