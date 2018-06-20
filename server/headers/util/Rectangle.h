#include <string>

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
        bool isInside(int pointX, int pointY);
        std::string getAsString();
        ~Rectangle();
    private:
        bool isXInside(int pointX);
        bool isYInside(int pointY);
};
#endif // RECTANGLE_H


