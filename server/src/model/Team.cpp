#include "model/Team.h"
#include "util/Formaciones.h"



Team::Team(int local, std::string name) {
    log("Team: Creando equipo.", LOG_INFO);
    this->players = {};
    this->local = local;
    this->name = name;
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

int Team::getAmountPlayers() {
    return players.size();
}

std::string Team::getName() {
    return name;
}

Team::~Team() {
    log("Team: Liberando memoria. Borrando jugadores...", LOG_INFO);
    // Deleting all players.
    // std::list<Player*>::iterator it;
    // for (it = this->players.begin(); it != this->players.end(); it++) {
    //     delete(*it);

    for (Player* p : players) {
        delete(p);
    }
    // Como la lista sigue llena, de punteros borrados, la vacio.
    this->players.clear();
    log("Team: Jugadores borrados. Memoria liberada.", LOG_INFO);
}

void Team::setFormacion(int formacion) {
    this->formacion = formacion;
}

void Team::order() {
    log("Team: Ordenando equipo...", LOG_DEBUG);
    int i = 0;
    for (Player* p : players) {
        p->setPosition(getFormation(this->local, this->formacion)[i]);
        // Coloca la posicion base de regreso
        p->setBasePosition(getFormation(this->local, this->formacion)[i]);
        i++;
    }
    log("Team: Equipo ordenado.", LOG_DEBUG);
}
