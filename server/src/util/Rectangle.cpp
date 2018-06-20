#include "util/Rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

bool Rectangle::isInside(int pointX, int pointY) {
    return this->isXInside(pointX) && this->isYInside(pointY);
}

bool Rectangle::isXInside(int pointX) {
    return pointX > this->x && ((this->x + this->width) - pointX) >= 0;
}

bool Rectangle::isYInside(int pointY) {
    return pointY > this->y && ((this->y + this->height) - pointY) >= 0;
}

std::string Rectangle::getAsString() {
    return std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->width) + ", " + std::to_string(this->height);
}

Rectangle::~Rectangle() {
}
