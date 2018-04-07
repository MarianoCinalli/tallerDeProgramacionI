#include "model/Team.h"

Team::Team() {
    this->players = {};
}

void Team::addPlayer(Player* player) {
    this->players.push_back(player);
}

std::list<Player*> Team::getPlayers() {
    return this->players;
}

Team::~Team() {
}
