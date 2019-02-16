//
// Created by nikita on 08.02.19.
//

#ifndef CSTRATEGY_KERNELSTRATEGY_H
#define CSTRATEGY_KERNELSTRATEGY_H

#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

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

    struct Robot {
        enum Role {
            Leader
        };

        std::string name_;
        int number_;

        Point position_;
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
class ComputerVisionModule {
public:
    bool IsNearTheEdge() {
        return false;
    }

    bool IsNormalTrajectory() {
        return false;
    }

    const double GetNewTrajectoryAngle() {
        return 0;
    }
};

//удалить
class MovementGraph {
public:
    bool Move(float x, float y, float theta) {
        return false;
    }

    void SetTheta(const double d) {

    }
};



class KernelStrategy {
public:
    bool Start();
    bool Pause();
    bool Terminate();
    strategy::Command WhatToDo();

private:
    strategy::Robot GetLeader();
    void UpdatePositions();
    void SprintLogic();

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

    std::vector<strategy::Robot>& robots_;

    //переписать на прокси
    LocalizationModule localizationModule{};
    ComputerVisionModule computerVisionModule{};
    MovementGraph movementGraph{};

    std::atomic<bool> is_terminated_{false};
    std::atomic<bool> is_paused_{false};
    std::vector<std::thread> threads_{};
};


#endif //CSTRATEGY_KERNELSTRATEGY_H
