#include "util/Coordinates.h"

Coordinates::Coordinates(int x, int y) {
    this->x = x;
    this->y = y;
}

Coordinates::~Coordinates() {
}

int Coordinates::getX() {
    return this->x;
}

int Coordinates::getY() {
    return this->y;
}

void Coordinates::incrementX() {
    this->x++;
}

void Coordinates::decrementX() {
    this->x--;
}

void Coordinates::incrementY() {
    this->y++;
}

void Coordinates::decrementY() {
    this->y--;
}
