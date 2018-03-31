#include "controller/actions/Accelerate.h"

Accelerate::Accelerate(int direction) {
    this->direction = direction;
}

void Accelerate::execute(Player* player) {
    player->accelerate(this->direction);
}

Accelerate::~Accelerate() {
}


