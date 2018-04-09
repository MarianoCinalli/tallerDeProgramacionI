#include "util/Coordinates.h"
#include <cmath>


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

void Coordinates::setCoord(Coordinates other){
  this->x = other.x;
  this->y = other.y;
}

int Coordinates::distanceTo(Coordinates* other){
  return sqrt(pow(this->x-other->x,2) + pow(this->y-other->y,2));
}

std::string Coordinates::toString() {
    return "x: " + std::to_string(this->x) + " y: " + std::to_string(this->y);
}
