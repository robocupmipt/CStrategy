//
// Created by nikita on 08.02.19.
//

#ifndef CSTRATEGY_KERNELSTRATEGY_H
#define CSTRATEGY_KERNELSTRATEGY_H

#include <string>
#include <vector>

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


class LocalizationModule { //to delete
public:
    std::vector<strategy::Point> GetCurrentPositions() {
        //pass
    }
};


class KernelStrategy {
public:
    strategy::Command WhatToDo();

private:
    strategy::Robot GetLeader();
    void UpdatePositions();

private:
    std::vector<strategy::Robot>& robots_;
    LocalizationModule localization{};
};


#endif //CSTRATEGY_KERNELSTRATEGY_H
