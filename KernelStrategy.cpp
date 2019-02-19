//
// Created by nikita on 08.02.19.
//

#include "KernelStrategy.hpp"


strategy::Robot KernelStrategy::GetLeader() {
    strategy::Robot best{};
    for (auto& current : robots_) {
        if (current.position_.y > best.position_.y ) {
            best = current;
        }
    }
    return best;
}

strategy::Command KernelStrategy::WhatToDo() {
    return {};
}

void KernelStrategy::UpdatePositions() {
    auto tmp_positions = localizationModule.GetCurrentPositions();
    for (auto& current : robots_) {
        auto iter = current.number_;
        current.position_ = tmp_positions[iter];
    }
}

bool KernelStrategy::Pause() {
    const bool answer = !is_paused_.load();
    is_paused_.store(true);
    return answer;
}

bool KernelStrategy::Start() {
    const bool answer = threads_.empty();
    const ProAngle horizontal(-60, 60, 10);
    const ProAngle vertical(-20, 20, 5);

    auto robot_routine = [&]() {
        while (true) {
            if (is_paused_.load()) {
                break;
            }
            if (is_terminated_.load()) {
                break;
            }

            for (double h_iter = horizontal.left_; h_iter < horizontal.right_;) {
                //head rotate in vertical direction
                for (double v_iter = vertical.left_; v_iter < vertical.right_;) {
                    //head rotate in horizontal direction
                    v_iter += vertical.step_;
                }
                h_iter += horizontal.step_;
            }

        }
    };
    threads_.emplace_back(robot_routine);
    return answer;
}

std::vector<double> KernelStrategy:ComputerVisionModule::getBallCentre(cv::Mat image)
	//Центр мяча(или иного ориентира) или пустой вектор, если его нет
	{
		std::vector<double> center = { 100,50 };//x,y
		return center;
	}
std::vector <double> KernelStrategy:ComputerVisionModule::getHorisontalLine(
        cv::Mat image)//горизонтальную линию, или пустой вектор, если ее нет 
	{
		std::vector <double> bx = { 0,0 };//k;b 
		return bx;т
	}
std::vector <double> KernelStrategy:ComputerVisionModule::getLeftVerticalLine(
    cv::Mat image)//левую вертикальную линию, или пустой вектор, если его нет 
	{
		std::vector <double> bx = { 0,0 };//k;b
		return bx;
	}
std::vector <double> KernelStrategy:ComputerVisionModule::getRightVerticalLine(
        cv::Mat image)//правую вертикальную линию, или пустой вектор, если его нет 
	{
		std::vector <double> bx = { 0,0 };//k;b 
		return bx;
	}
int KernelStrategy:ComputerVisionModule::BallCondition(double center_y, int allowed_gamma = 100)
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
int KernelStrategy:ComputerVisionModule::LineCondition(std::vector <double> left_line,
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
int KernelStrategy:ComputerVisionModule::BackCondition(
    std::vector <double> horisontal_line, std::vector <double> center)
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
int KernelStrategy:ComputerVisionModule::ShouldTurn(
    cv::Mat image, int allowed_gamma = 100)
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
		return 0;
	}
const double KernelStrategy:ComputerVisionModule::GetNewTrajectoryAngle(
    cv::Mat image, int allowed_gamma,double current_angle, double delta)//current_angle текущий угол
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




bool KernelStrategy::Terminate() {
    const bool answer = !is_terminated_.load();
    is_terminated_.store(true);
    return answer;
}

void KernelStrategy::SprintLogic() {
    int total_amount_of_metres_passed = 0;
    while (!computerVisionModule.IsNearTheEdge()) {
        movementGraph.Move(1, 0, 0);
        total_amount_of_metres_passed++;
        while (!computerVisionModule.IsNormalTrajectory()) {
            const double angle = computerVisionModule.GetNewTrajectoryAngle();
            movementGraph.SetTheta(angle);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    movementGraph.Move(-total_amount_of_metres_passed, 0, 0);
}
