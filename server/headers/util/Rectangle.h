#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/Velocity.h"
#include <string>
#include <iostream>

#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle {
    private:
        int x;
        int y;
        int width;
        int height;
    public:
        Rectangle(int x, int y, int width, int height);
        bool isCloseToBordersInX(int x);
        bool isCloseToBordersInY(int y);
        bool isInside(int pointX, int pointY);
        bool isVelocityPointingInside(Velocity* velocity, Coordinates* coordinates);
        std::string getAsString();
        ~Rectangle();
    private:
        bool isXInside(int pointX);
        bool isYInside(int pointY);
        bool isCloseTo(int a, int b);
};
#endif // RECTANGLE_H
