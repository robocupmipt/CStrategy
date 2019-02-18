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


int X_DIM = 640;//размерность изображения
int Y_DIM = 480;//размерность изображения
//удалить
class ComputerVisionModule {
public:

	std::vector<double> getBallCentre(cv::Mat image)//Центр мяча
	{
		std::vector<double> center = { 100,50 };//x,y
		return center;
	}
	std::vector <double> getHorisontalLine(cv::Mat image)//вертикальную линию
	{
		std::vector <double> bx = { 0,0 };//k;b 
		return bx;//пустой вектор, если Горизонтальной линии нет
	}
	std::vector <double> getLeftVerticalLine(cv::Mat image)//левую горизонтальную линрию
	{
		std::vector <double> bx = { 0,0 };//k;b
		return bx;//пустой вектор, если левой линии нет
	}
	std::vector <double> getRightVerticalLine(cv::Mat image)//правую горизонтальную линию
	{
		std::vector <double> bx = { 0,0 };//k;b 
		return bx;//пустой вектор, если правой линии нет
	}
	int BallCondition(double center_y, int allowed_gamma = 100)
	{
		if (center_y < double(Y_DIM / 2 - allowed_gamma))//центр мяча смещен влево
		{
			return -1;
		}
		if (center_y > double(Y_DIM / 2 + allowed_gamma))//центр мяча смещен вправо
		{
			return 1;
		}
		return 0;//центр мяча не смещен
	}
	int LineCondition(std::vector <double> left_line,
		std::vector <double> right_line)
	{
		if (left_line.empty() and !right_line.empty())//левую линию видим, правую нет
		{
			return 1;//направо
		}
		if (right_line.empty() and !left_line.empty())//левую линию видим, правую нет
		{
			return -1;//направо
		}
		return 0;
	}
	int BackCondition(std::vector <double> horisontal_line, std::vector <double> center)
	{
		///если горизонтальную линию не видим, или видим, но она на всем протяжении кадра пролегает за мячом(то есть ее y всегда выше, чем у мяча)
		
		///тогда 1
		///иначе 0
		double line_y_min = std::min(horisontal_line[1], horisontal_line[1] + horisontal_line[0] * X_DIM);
		if (horisontal_line.empty() or line_y_min > center[1])
		{
			return 1;
		}
		return 0;
	}
	int ShouldTurn(cv::Mat image, int allowed_gamma = 100)
	//allowed_gamma - то, на сколько центр мяча может отклоняться от центра картинки, чтобы это еще было норм
	//Возвращаемое значение : -1 -влево ; 0 - не должен; 1 - вправо;
	//999 - надо пятиться
	{

		std::vector <double> left_line = getLeftVerticalLine(image);
		std::vector <double> right_line = getRightVerticalLine(image);
		std::vector <double> horisontal_line = getHorisontalLine(image);
		std::vector <double> center = getBallCentre(image);
		int back_condition = BackCondition(horisontal_line, center);

		int line_condition = LineCondition(left_line, right_line);
		int ball_condition = BallCondition(center[1], allowed_gamma);
		if (back_condition)//пятиться
		{
			return 999;
		}
		if (std::abs(ball_condition) > 0)//сначала проверяем условие по мячу
		{
			return ball_condition;
		}
		if (std::abs(line_condition) > 0)//а потом по линиям
		{
			return line_condition;
		}
		return false;
	}

	const double GetNewTrajectoryAngle(cv::Mat image, int allowed_gamma,
		double current_angle, double delta)//current_angle текущий угол
		//delta это элементарный угол поворота ( мы можем поворачиваться максимум на 1 элементарный угол)
	{
		int to_turn = ShouldTurn(image, allowed_gamma);
		if (to_turn > 990)//значит, надо возвращать 999, а 999 означает, что надо пятиться
		{
			return 999;
		}
		else//поворачиваемся на delta вправо или влево
		{
			return current_angle + to_turn * delta;
		}
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
