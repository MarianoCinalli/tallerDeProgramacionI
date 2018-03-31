#include "controller/actions/Decelerate.h"

Decelerate::Decelerate(int direction) {
    this->direction = direction;
}

void Accelerate::excecute(Player* player) {
    player->decelerate(this->direction);
}

Decelerate::~Decelerate() {
}


