#include "controller/actions/Decelerate.h"

Decelerate::Decelerate(int direction) {
    this->direction = direction;
}

void Decelerate::execute(Player* player) {
    log("Decelerate: Frenando al jugador.", LOG_DEBUG);
    player->decelerate(this->direction);
}

Decelerate::~Decelerate() {
}


