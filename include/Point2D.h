//
// Created by nikita on 28.03.19.
//

#ifndef STRATEGYMODULE_POINT2D_H
#define STRATEGYMODULE_POINT2D_H

#pragma once

struct Point2D {
    Point2D(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double x;
    double y;
};

#endif //STRATEGYMODULE_POINT2D_H
