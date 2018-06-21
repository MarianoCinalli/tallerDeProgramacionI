#include "util/Rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

bool Rectangle::isCloseToBordersInX(int x) {
    bool isTrue = this->isCloseTo(x, this->x) || this->isCloseTo(x, this->x + this->width);
    /*if (isTrue) {
        std::cout << "Jugador en: " << x << "  x1: " << this->x <<  " x2: " << this->x + this->width << std::endl;
    }*/
    return isTrue;
}

bool Rectangle::isCloseToBordersInY(int y) {
    bool isTrue = this->isCloseTo(y, this->y) || this->isCloseTo(y, this->y + this->height);
    /*if (isTrue) {
        std::cout << "Jugador en: " << y << "  x1: " << this->y <<  " x2: " << this->y + this->height << std::endl;
    }*/
    return isTrue;
}

bool Rectangle::isCloseTo(int a, int b) {
    return (a > (b - AREAS_MARGIN)) && (a < (b + AREAS_MARGIN));
}

bool Rectangle::isVelocityPointingInside(Velocity* velocity, Coordinates* coordinates) {
    bool result = true;
    if (this->isCloseTo(coordinates->getX(), this->x)) {
        result = velocity->getComponentX() > 0;
    } else if (this->isCloseTo(coordinates->getX(), this->x + this->width)) {
        result = velocity->getComponentX() < 0;
    } else if (this->isCloseTo(coordinates->getY(), this->y)) {
        result = velocity->getComponentY() > 0;
    } else if (this->isCloseTo(coordinates->getY(), this->y + this->height)) {
        result = velocity->getComponentY() < 0;
    }
    return result;
}

bool Rectangle::isInside(int pointX, int pointY) {
    return this->isXInside(pointX) && this->isYInside(pointY);
}

bool Rectangle::isXInside(int pointX) {
    return pointX > this->x && ((this->x + this->width) > pointX);
}

bool Rectangle::isYInside(int pointY) {
    return pointY > this->y && ((this->y + this->height) > pointY);
}

std::string Rectangle::getAsString() {
    return std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->width) + ", " + std::to_string(this->height);
}

Rectangle::~Rectangle() {
}
