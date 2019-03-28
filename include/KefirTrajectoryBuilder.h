//
// Created by nikita on 28.03.19.
//

#ifndef STRATEGYMODULE_KEFIRTRAJECTORy_ballUILDER_H
#define STRATEGYMODULE_KEFIRTRAJECTORy_ballUILDER_H
#pragma once

#include "Point2D.h"

class KefirTrajectory_balluilder{
private :
    /*Constants*/
    /*TODO: fix the constants to the field's constants */
    const int WIND_X = 800;
    const int WIND_Y = 600;

    const int GOAL_POS = 100;
    const int GOAL_LEN = 300;

    const int BALL_RADIUS   = 20;
    const int CIRCLE_RADIUS = 50;
    const int ROBOT_SIZE    = 50;

public:
    std::vector<Point2d> find_trajectory (Point2D robot, Point2D ball, int max_step)
    {
        double x_robot = robot.x;
        double y_robot = robot.y;

        double x_ball = ball.x;
        double y_ball = ball.y;

        std::vector<Point2D> trajectory();

        trajectory.push_back (Point2d(x_robot, y_robot));

        //-----------------------------------------------------------
        //find starting point on the circle

        int x_ball_relative = x_ball - x_robot;
        int y_ball_relative = y_ball - y_robot;

        int r    = CIRCLE_RADIUS;
        float leng = sqrt (x_ball_relative*x_ball_relative + y_ball_relative*y_ball_relative);

        float beta  = asin (float (y_ball_relative) / leng);
        float alpha = asin (float (r) / leng);

        if (x_ball < x_robot)
        {
            float sx = 0;
            float sy = 0;

            if (y_robot + (y_robot - GOAL_POS - int (GOAL_LEN / 2)) * (x_robot - x_ball) / (WIND_X - x_ball) > y_ball)
            {
                sx = -leng * cos (alpha + beta) * cos (alpha) + x_robot;
                sy = leng * sin (alpha + beta) * cos (- alpha) + y_robot;
            }

            else
            {
                alpha = -alpha;

                sx = - leng * cos (alpha + beta) * cos (alpha) + x_robot;
                sy = leng * sin (alpha + beta) * cos (- alpha) + y_robot;
            }

            trajectory.push_back (Point2d (int (sx), int (sy)));
        }

        //-----------------------------------------------------------
        //find kick point on the circle
        int goal_ball_x = x_ball - WIND_X;
        int goal_ball_y = y_ball - GOAL_POS - int (GOAL_LEN / 2);

        int length_gb = int(sqrt(goal_ball_x*goal_ball_x + goal_ball_y*goal_ball_y));

        int kick_point_x = x_ball + int (CIRCLE_RADIUS * goal_ball_x / length_gb);
        int kick_point_y = y_ball + int (CIRCLE_RADIUS * goal_ball_y / length_gb);

        trajectory.push_back (Point2d (kick_point_x, kick_point_y));
        trajectory.push_back (Point2d (WIND_X, GOAL_POS + int (GOAL_LEN / 2)));

        return trajectory;
    }
};

#endif //STRATEGYMODULE_KEFIRTRAJECTORy_ballUILDER_H
