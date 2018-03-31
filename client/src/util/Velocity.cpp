#include "util/Velocity.h"

Velocity::Velocity(int x, int y) {
    this->x = x;
    this->y = y;
}

void Velocity::accelerate(int direction, int amount) {
    this->changeOnDirection(direction, amount);
}

void Velocity::decelerate(int direction, int amount) {
    this->changeOnDirection(direction, (-1) * amount);
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
