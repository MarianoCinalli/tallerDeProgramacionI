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
    // Deleting all players.
    std::list<Player*>::iterator it;
    for (it = this->players.begin(); it != this->players.end(); it++) {
        delete(*it);
    }
    // Como la lista sigue llena, de punteros borrados, la vacio.
    this->players.clear();
}
