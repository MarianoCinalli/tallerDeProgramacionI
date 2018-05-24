#include "model/Team.h"
// #include "util/Formaciones.h"

Team::Team() {
    log("Team: Creando equipo.", LOG_INFO);
    this->players = {};
    log("Team: Equipo creado.", LOG_INFO);
}

void Team::addPlayer(Player* player) {
    log("Team: Agregando jugador.", LOG_INFO);
    this->players.push_back(player);
    log("Team: Jugador agregado.", LOG_INFO);
}

std::list<Player*> Team::getPlayers() {
    return this->players;
}

Team::~Team() {
    log("Team: Liberando memoria. Borrando jugadores...", LOG_INFO);
    for(Player* p : players){
      delete(p);
    }
    // Como la lista sigue llena, de punteros borrados, la vacio.
    this->players.clear();
    log("Team: Jugadores borrados. Memoria liberada.", LOG_INFO);
}
