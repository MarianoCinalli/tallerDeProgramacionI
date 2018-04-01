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

void Coordinates::addX(int amount) {
    this->x += amount;
}

void Coordinates::subtractX(int amount) {
    this->x -= amount;
}

void Coordinates::addY(int amount) {
    this->y += amount;
}

void Coordinates::subtractY(int amount) {
    this->y -= amount;
}
