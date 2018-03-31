#include "controller/actions/Decelerate.h"

Decelerate::Decelerate(int direction) {
    this->direction = direction;
}

void Decelerate::execute(Player* player) {
    player->decelerate(this->direction);
}

Decelerate::~Decelerate() {
}


