#include "util/Rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

bool Rectangle::isCloseToBordersInX(int x) {
    return this->isCloseTo(x, this->x) || this->isCloseTo(x, this->x + this->width);
}

bool Rectangle::isCloseToBordersInY(int y) {
    return this->isCloseTo(y, this->y) || this->isCloseTo(y, this->y + this->height);
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

// Devuelve el punto mas lejano, con respecto a otro.
Coordinates* Rectangle::getFurthestVertex(Coordinates* point) {
    int middleX = this->x + this->width / 2;
    int middleY = this->y + this->height / 2;
    int x = point->getX();
    int y = point->getY();
    if (point->getY() < middleY) {
        y = this->y + this->height;
    } else {
        y = this->y;
    }
    if (point->getX() < middleX) {
        x = this->x + this->width;
    } else {
        x = this->x;
    }
    Coordinates* coordinates = new Coordinates(x, y);
    return coordinates;
}

// Devuelve el punto mas lejano, con respecto a otro, en un lado particular del rectangulo.
Coordinates* Rectangle::getFurthestVertex(Coordinates* point, bool onRightSide) {
    Coordinates* coordinates = this->getFurthestVertex(point);
    if (onRightSide) {
        coordinates->setX(this->x + this->width);
    } else {
        coordinates->setX(this->x);
    }
    return coordinates;
}

Rectangle::~Rectangle() {
}
