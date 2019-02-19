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
//удалить
class ComputerVisionModule {
public:
	std::vector <std::vector<double>> getLines(cv::Mat image)
	{
		std::vector<double> line1 = {0,0};//k;b
		std::vector <std::vector<double>> answ;
		answ.push_back(line1);
		return answ;
	}
	std::vector<double> getBallCentre(cv::Mat image)
	{//центр мяча(или пустой вектор, если его нет)
		std::vector<double> center = { 100,50 };//x,y
		return center;
	}
	std::vector <double> getHorisontalLine(cv::Mat image)
		//горизонтальную линию(или пустой вектор, если её нет)
	{
		std::vector <double> bx = { 0,0 };//k;b 
		return bx;//пустой вектор, если Горизонтальной линии нет
	}
	std::vector <double> getLeftVerticalLine(cv::Mat image)
		//левую вертикальную линию(или пустой вектор, если её нет)
	{
		std::vector <double> bx = { 0,0 };//k;b
		return bx;//пустой вектор, если левой линии нет
	}
	std::vector <double> getRightVerticalLine(cv::Mat image)
		//правую вертикальную линию(или пустой вектор, если её нет)
	{
		std::vector <double> bx = { 0,0 };//k;b 
		return bx;
	}
	int BallCondition(double center_y, int allowed_gamma = 100)
	{      //центр мяча смещен влево больше чем на allowed_gamma -возвращаем -1,
		///если вправо то 1, если ни то, ни то, то 0

		return 0;
	}
	int LineCondition(std::vector <double> left_line,
		std::vector <double> right_line)
	{
		//левую линию видим, правую нет - направо ; и наоборот

		return 0;
	}
	int BackCondition(std::vector <double> horisontal_line, std::vector <double> center)
	{
		///если горизонтальную линию не видим, 
		//или видим, но она на всем протяжении кадра пролегает за мячом(то есть ее y всегда выше, чем у мяча)
		
		///тогда 1
		///иначе 0
		return 0;
	}
	int ShouldTurn(cv::Mat image, int allowed_gamma = 100)
	//allowed_gamma - то, на сколько центр мяча может отклоняться от центра картинки, чтобы это еще было норм
	//Возвращаемое значение : -1 -влево ; 0 - не должен; 1 - вправо;
	//999 - надо пятиться
	{
		return 0;
	}

	const double GetNewTrajectoryAngle(cv::Mat image, int allowed_gamma,
		double current_angle, double delta)//current_angle текущий угол
		//delta это элементарный угол поворота ( мы можем поворачиваться максимум на 1 элементарный угол)
	{
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
