#include "pch.h"
#include "KernelStrategy.h"
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
//X_DIM,Y_DIM,pattern_size - в хедере определены
std::vector<std::vector<double> > ComputerVisionModule::getLines(cv::Mat image)
{
	///ФУНКЦИЯ ДЛЯ ДЕТЕКЦИИ ЛИНИЙ
	std::vector<double> line1 = { 0,0 };//k;b
	std::vector <std::vector<double>> answ;
	answ.push_back(line1);
	return answ;
}
std::vector<std::vector<double> > ComputerVisionModule::processLines(
	std::vector<std::vector<double> > lines, int X_checkpoint = X_DIM / 2,
	int Y_checkpoint = Y_DIM / 2)
	///Возвращает лучшую горизонтальную линию, лучшую левую вертикальную линию и лучшую правую вертикальную линию
	///Линия считается левой вертикальной, если значения ее координаты X при всех Y меньше, чем X_checkpoint
	///и правой вертикальной, если наоборот, значения X при всех Y больше, чем X_checkpoint
	//а если ни то, ни  то, то горизонтальной
	///Лучшая левая линия - та, у которой X при Y = Y_DIM//2 максимально, правая - у которой X минимально
	///лучшая горизонтальная линия - та, у которой при X=X_DIM//2 Y наоборот, минимально
{
	std::vector<double> horisontal_line;
	std::vector <double> left_line;
	std::vector <double> right_line;
	std::vector<std::vector<double> > answer;
	double k,b,x1,x2,left_xmax,right_xmin,center_ymin;
	double total_left_xmax = 0;//Если мы встретили параметр total_left_xmax больше заданного, обновляем левую линию
	double total_right_xmin = 9999;//Если мы встертили total_right_xmin меньше заданного, обновляем правую линию
	double total_center_ymin = 9999;//Если мы встретили total_center_ymin меньше заданного, обновляем горизонтальную линию
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		k = lines[i][0];
		b = lines[i][1];

		///X максимально или миннимально там, где k*x+b = 0 или k*x+b=Y_dim

		x1 = (-b) / k;
		x2 = (Y_DIM - b) / k;

		if (std::max(x1, x2) < X_checkpoint)
		{
			left_xmax = (Y_checkpoint - k) / b;
			if (left_xmax > total_left_xmax)
			{
				total_left_xmax = left_xmax;
				left_line = lines[i];
			}
		}
		else if (std::min(x1, x2) > X_checkpoint)
		{
			right_xmin = (Y_checkpoint - k) / b;
			if (right_xmin < total_right_xmin)
			{
				total_right_xmin = right_xmin;
				right_line = lines[i];
			}
		}
		else
		{
			center_ymin = k * X_checkpoint + b;
			if (center_ymin < total_center_ymin)
			{
				total_center_ymin = center_ymin;
				horisontal_line = lines[i];
			}
		}
	}
	answer.push_back(left_line);
	answer.push_back(horisontal_line);
	answer.push_back(right_line);
	return answer;

}

std::vector<double> ComputerVisionModule::getChessboardCentre(
	cv::Mat image, cv::Size patternsize = pattern_size)
//interior number of corners)
//ФУНКЦИЯ ДЛЯ ДЕТЕКЦИИ шахматной доски - ищем среднее значение x для всех найденных углов, на него потом будем ориентироваться)
//(Если часть углов не попадает в кадр, то значение может быть смещено по сравнению с реальным, 
//на это потом надо будет сделать поправку, если не будет более приоритетных задач
//- или же просто сделать allowed_gamma достаточно маленьким для того, чтобы это не имело значения
{
	int tmp_x;
	int tmp_y;
	std::vector <cv::Point2f> corners;
	for (int i = 3; i <= patternsize.width; i++)
	{
		for (int j = 3; j <= patternsize.height; j++)
		{
			bool patternFound = findChessboardCorners(image, cv::Size(i, j), corners,
				cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
				+ cv::CALIB_CB_FAST_CHECK);
			if (patternFound)
			{
				tmp_x = 0;
				tmp_y = 0;
				for (unsigned int k = 0; k < corners.size(); k++)
				{
					tmp_x = tmp_x + corners[k].x;
					tmp_y = tmp_y + corners[k].y;
				}
				tmp_x = tmp_x / corners.size();
				tmp_y = tmp_y / corners.size();
			}
		}
	}
	std::vector <double> answ;
	answ.push_back(tmp_x);
	answ.push_back(tmp_y);
	return answ;
}

int ComputerVisionModule::ChessboardCondition(double center_y, int allowed_gamma = 100)
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
int ComputerVisionModule::LineCondition(std::vector <double> left_line,
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
int ComputerVisionModule::BackCondition(
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
int ComputerVisionModule::ShouldTurn( int allowed_gamma = 100, int X_checkpoint = X_DIM / 2, int Y_checkpoint = Y_DIM / 2)
	//allowed_gamma - то, на сколько центр мяча может отклоняться от центра картинки, чтобы это еще было норм
	//Возвращаемое значение : -1 -влево ; 0 - не должен; 1 - вправо;
	//999 - надо пятиться
{
	//СЮДА ДОБАВИТЬ СКРИПТ ДЛЯ ПОЛУЧЕНИЯ ИЗОБРАЖЕНИЯ!!! ЕГО МЫ ПОЛУЧАЕМ КАЖДЫЙ РАЗ В ЭТОЙ ФУНКЦИИ!!!
	cv::Mat image;
	std::vector<std::vector <double>> raw_lines = ComputerVisionModule::getLines(image);
		std::vector <std::vector<double>> processed_lines = ComputerVisionModule::processLines(
			raw_lines, X_checkpoint = X_checkpoint, Y_checkpoint = Y_checkpoint);
	std::vector<double> left_line = processed_lines[0];
	std::vector<double> horisontal_line = processed_lines[1];
	std::vector<double> right_line = processed_lines[2];

	std::vector <double> center = getChessboardCentre(image);
	int back_condition = BackCondition(horisontal_line, center);

	int line_condition = LineCondition(left_line, right_line);
	int chessboard_condition = ChessboardCondition(center[1], allowed_gamma);
	if (back_condition)//пятиться
	{
		return 999;
	}
	if (std::abs(chessboard_condition) > 0)//сначала проверяем условие по мячу
	{
		return chessboard_condition;
	}
	if (std::abs(line_condition) > 0)//а потом по линиям
	{
		return line_condition;
	}
	return 0;
}

const double ComputerVisionModule::GetNewTrajectoryAngle(
	int turn_command, double current_angle, double delta=0.01)//current_angle текущий угол
		//delta это элементарный угол поворота ( мы можем поворачиваться максимум на 1 элементарный угол каждый раз)
{
	if (turn_command > 990)//значит, надо возвращать 999, а 999 означает, что надо пятиться
	{
		return 999;
	}
	else//поворачиваемся на delta вправо или влево
	{
		return current_angle + turn_command * delta;
	}
}




bool KernelStrategy::Terminate() {
	const bool answer = !is_terminated_.load();
	is_terminated_.store(true);
	return answer;
}

void KernelStrategy::SprintLogic() {
	float total_amount_of_metres_passed = 0;
	int turn_command = -9;
	bool end_loop = false;
	double current_angle = 0;
	while (!end_loop) 
	{
		turn_command = computerVisionModule.ShouldTurn();
		while (turn_command<999)
		{
			movementGraph.Move(1, 0, 0);
			total_amount_of_metres_passed++;
			while (std::abs(turn_command)==1)
			{
				const double angle = computerVisionModule.GetNewTrajectoryAngle(
					turn_command,current_angle);
				movementGraph.SetTheta(angle);
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
		}
	}
	movementGraph.Move(-total_amount_of_metres_passed, 0, 0);
}
//если добавить main, То компилится
//int main() 
//{
//	return 0;
//}