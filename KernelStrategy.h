#ifndef CSTRATEGY_KERNELSTRATEGY_H
#define CSTRATEGY_KERNELSTRATEGY_H

#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"

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


int X_DIM = 640;//размерность изображения
int Y_DIM = 480;//размерность изображения
cv::Size pattern_size = cv::Size(8, 5);//размерность доски
//удалить
class ComputerVisionModule {
public:
	static std::vector <std::vector<double>> getLines(cv::Mat image);

	static std::vector <std::vector<double>> processLines(
		std::vector<std::vector<double> > lines,
		int X_checkpoint,
		int Y_checkpoint);

	static std::vector<double> getChessboardCentre(cv::Mat image, cv::Size pattern_size);
	static int ChessboardCondition(double center_y, int allowed_gamma);
	static int LineCondition(std::vector <double> left_line, std::vector <double> right_line);
	static int BackCondition(std::vector <double> horisontal_line, std::vector <double> center);
	static int ShouldTurn(int allowed_gamma,int X_checkpoint, int Y_checkpoint);
	static const double GetNewTrajectoryAngle(int turn_command, double current_angle, double delta);
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