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
    return ((this->x + this->width) - pointX) >= 0;
}

bool Rectangle::isYInside(int pointY) {
    return ((this->y + this->height) - pointY) >= 0;
}

Rectangle::~Rectangle() {
}
