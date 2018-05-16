#include "controller/actions/Accelerate.h"

Accelerate::Accelerate(int direction) {
    this->direction = direction;
}

void Accelerate::execute(Player* player) {
    log("Accelerate: Acelerando al jugador.", LOG_DEBUG);
    player->accelerate(this->direction);
}

bool Accelerate::valid(Player* player) {
    return true;
}

std::string Accelerate::getCommand() {
    return "Accelerate";
}

std::string Accelerate::getParameters() {
    return std::to_string(this->direction);
}

Accelerate::~Accelerate() {
}
