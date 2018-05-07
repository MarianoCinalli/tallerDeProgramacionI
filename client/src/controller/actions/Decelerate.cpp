#include "controller/actions/Decelerate.h"

Decelerate::Decelerate(int direction) {
    this->direction = direction;
}

void Decelerate::execute(Player* player) {
    log("Decelerate: Frenando al jugador.", LOG_DEBUG);
    player->decelerate(this->direction);
}

bool Decelerate::valid(Player* player){
  return true;
}

std::string Decelerate::getCommand() {
    return "decelerate";
}

Decelerate::~Decelerate() {
}
