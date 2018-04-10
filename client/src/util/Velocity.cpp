#include "util/Velocity.h"

Velocity::Velocity(int x, int y) {
    this->x = x;
    this->y = y;
}

int Velocity::getComponentX() {
    return this->x;
}

int Velocity::getComponentY() {
    return this->y;
}

void Velocity::accelerate(int direction, int amount) {
    this->changeOnDirection(direction, amount);
}

void Velocity::decelerate(int direction, int amount) {
    this->changeOnDirection(direction, (-1) * amount);
}

void Velocity::stop(){
  this->x = 0;
  this->y = 0;
}

bool Velocity::isZero() {
    return ((this->x == 0) && (this->y == 0));
}


// Como el origen esta arriba a la izquierda.
// Para la derecha y abajo suma si acelera, resta si desacelera.
// Para la izquierda y arriba resta si acelera, suma si desacelera.
void Velocity::changeOnDirection(int direction, int amount) {
    switch(direction) {
        case PLAYER_ORIENTATION_LEFT:
            this->x -= amount;
            break;
        case PLAYER_ORIENTATION_UP:
            this->y -= amount;
            break;
        case PLAYER_ORIENTATION_RIGHT:
            this->x += amount;
            break;
        case PLAYER_ORIENTATION_DOWN:
            this->y += amount;
            break;
    }
}

Velocity::~Velocity() {
}

std::string Velocity::toString() {
    return "x: " + std::to_string(this->x) + " y: " + std::to_string(this->y);
}
